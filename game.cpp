#include "Game.h"

void Game::run() {
	// SDL initializations.
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "Initialization failed.\nError: " << SDL_GetError() << "\n";
	}
	if (!(IMG_Init(IMG_INIT_PNG))) {
		std::cout << "Could not initialize image.\nError: " << SDL_GetError() << "\n";
	}

	if (displayMainMenu() == 0) {
		gameIsRunning = true;

		// Class responsible for automatically generating new tiles as the player moves.
		floor = Tiles("resources/textures/ground.png");
		floor.create();

		initiateEntities();
		handleEvents();
	}
}

void Game::stop() {
	// Destroys and cleans the window.
	window.cleanUp();

	// Quits SDL.
	SDL_Quit();
}

int Game::displayMainMenu() {
	SDL_SetWindowIcon(window.getWindow(), IMG_Load("resources/gfx/game_icon.png"));

	Entity mainMenu(Position(0, 0), "resources/gfx/mainmenu.png", 1280, 720, 1);
	bool displayMenu = true;
	while (displayMenu) {
		while (SDL_PollEvent(&event)) {
			if (pressedKey(SDLK_e)) {
				SDL_DestroyTexture(mainMenu.getTexture());
				displayMenu = false;
				break;
			}

			if (pressedKey(SDLK_ESCAPE)) {
				displayMenu = false;
				stop();
				return 1;
			}
		}
		window.render(mainMenu);
		window.display();
	}

	return 0;
}

void Game::initiateEntities() {
	playerOne = Entity(Position(SCREEN_CENTER_X, SCREEN_CENTER_Y), "resources/textures/gus_front.png", 64, 64);
	playerTwo = Entity(Position(playerOne.getX() - 40, playerOne.getY() + 10), "resources/textures/walt_front.png", 64, 64);
	dialogueBox = Entity(Position(playerTwo.getX() + 24, playerTwo.getY() - 40), "resources/textures/dialogue.png", 67, 36);
	chickenPopUp = Entity(Position(SCREEN_CENTER_X, SCREEN_CENTER_Y + 60), "resources/textures/chicken.png", 64, 36);
	borrowPopUp = Entity(Position(SCREEN_CENTER_X - 22, SCREEN_CENTER_Y + 60), "resources/textures/borrow.png", 104, 32);
	building = Entity(Position(150, 16), "resources/textures/los_food.png", 128, 72, 6);
	background = Entity(Position(-50, 0), "resources/textures/bg_new.png", 752, 288, 2);
	balance = Entity(Position(2, 2), balanceTextures[0], 64, 36, 4);
	quitText = Entity(Position(0, 334), "resources/textures/esc.png", 104, 32, 2);
	cppIcon = Entity(Position(-10, 14), "resources/textures/cpp.png", 150, 50, 6);
	wing = Entity(wingPosition, "resources/textures/wing.png", 32, 32, 2);

	players.push_back(&playerOne);
	players.push_back(&playerTwo);

	playerTwoEntities.push_back(&playerTwo);
	playerTwoEntities.push_back(&dialogueBox);
}

void Game::renderUIElements() {
	// Adds components to the underlying vector of class RenderEntities.
	RenderEntities::add(&background);
	RenderEntities::add(&cppIcon);
	RenderEntities::add(&building);
	RenderEntities::add(&balance);
	RenderEntities::add(floor.getFirstLevel());
	RenderEntities::add(floor.getSecondLevel());
	RenderEntities::add(&quitText);

	window.render(RenderEntities::get());
}

void Game::handleEvents() {
	const int MIN_CLICKS = 0;

	while (gameIsRunning) {
		window.clear();
		RenderEntities::clear();

		while (SDL_PollEvent(&event)) {
			if (pressedKey(SDLK_ESCAPE)) {
				gameIsRunning = false;
				break;
			}

			// Lets playerOne borrow money from playerTwo.
			if (playersAreClose()) {
				window.render(borrowPopUp);
				if (clicks > MIN_CLICKS) {
					if (pressedKey(SDLK_e)) {
						clicks--;
						balance.setTexture(balanceTextures[clicks]);
					}
				}
			}

			// Checks if playerOne is withing bounds of the door so he can buy chicken wings.
			if (playerOneByDoor()) {
				renderChickenWings();
			}
		}

		getPlayerOneKeyInputs();
		getPlayerTwoKeyInputs();

		update();

		window.display();
	}
}

void Game::update() {
	renderUIElements();

	// Makes players appear in order when moving up and down.
	for (auto& player : players) {
		std::sort(players.begin(), players.end(), Entity::cmp());
		window.render(*player);
	}

	changePlayerTextures();

	if (playerOneByDoor()) {
		window.render(chickenPopUp);
	}

	for (auto& w : wingCounter) {
		window.render(w);
	}
}

void Game::renderChickenWings() {
	const int MAX_CLICKS = 5;

	// Render a gray pop-up if player reaches maximum amount of clicks.
	(clicks == MAX_CLICKS)
		? chickenPopUp.setTexture("resources/textures/chicken_null.png")
		: chickenPopUp.setTexture("resources/textures/chicken.png");

	if (clicks < MAX_CLICKS) {
		if (pressedKey(SDLK_e)) {
			// Organizes so there are only 10 pieces of chicken wings per row.
			if (wing.getX() <= wingOrigin.x - (32 * 9)) {
				wingPosition.x = wingOrigin.x + 32;
				wingPosition.y += 32;
			}

			// Stores chicken wing entity in vector and moves next to the left.
			wingCounter.push_back(wing);
			wingPosition.x -= 32;
			wing.setPos(wingPosition.x, wingPosition.y);

			clicks++;
			balance.setTexture(balanceTextures[clicks]);
		}
	}
}

bool Game::playersAreClose() {
	const int PLAYER_DISTANCE = 40;

	if (playerDistanceX <= PLAYER_DISTANCE && playerDistanceY <= PLAYER_DISTANCE) {
		return true;
	}

	return false;
}

bool Game::playerOneByDoor() {
	if (building.getX() > 54 && building.getX() <= 80 && playerOne.getY() <= 80) {
		return true;
	}

	return false;
}

// Reads key input from user.
bool Game::pressedKey(const SDL_Keycode button) {
	bool isPressed = false;

	if (event.type != SDL_KEYDOWN) {
		return false;
	}
	else {
		if (event.key.keysym.sym == button) {
			return true;
		}
	}

	return false;
}

void Game::getPlayerOneKeyInputs() {
	const int MIN_Y = 70, MAX_Y = 150;

	entitiesToMove.clear();

	//Movement for player one.
	if (keystates[SDL_SCANCODE_W]) {
		playerOne.setTexture("resources/textures/gus_back.png");

		// Move playerOne.
		(playerOne.getY() == MIN_Y)
			? playerOne.move(0.0f, 0.0f)
			: playerOne.move(0.0f, -moveSpeed);
	}
	if (keystates[SDL_SCANCODE_A]) {
		playerOne.setTexture("resources/textures/gus_left.png");

		// Stores all entities which are to be moved.
		entitiesToMove.add(&cppIcon, (moveSpeed / 4), 0.0f);
		entitiesToMove.add(&building, (moveSpeed / 2), 0.0f);
		entitiesToMove.add(floor.getFirstLevel(), moveSpeed, 0.0f);
		entitiesToMove.add(floor.getSecondLevel(), moveSpeed, 0.0f);
		entitiesToMove.add(playerTwoEntities, moveSpeed, 0.0f);
		entitiesToMove.add(&background, (moveSpeed / 8), 0.0f);

		// Moves everything else in opposite direction of playerOne's direction.
		entitiesToMove.move();
	}
	if (keystates[SDL_SCANCODE_S]) {
		playerOne.setTexture("resources/textures/gus_front.png");

		// Move playerOne.
		(playerOne.getY() == MAX_Y)
			? playerOne.move(0.0f, 0.0f)
			: playerOne.move(0.0f, moveSpeed);
	}
	if (keystates[SDL_SCANCODE_D]) {
		playerOne.setTexture("resources/textures/gus_right.png");

		// Stores all entities which are to be moved.
		entitiesToMove.add(&cppIcon, -(moveSpeed / 4), 0.0f);
		entitiesToMove.add(&building, -(moveSpeed / 2), 0.0f);
		entitiesToMove.add(floor.getFirstLevel(), -moveSpeed, 0.0f);
		entitiesToMove.add(floor.getSecondLevel(), -moveSpeed, 0.0f);
		entitiesToMove.add(playerTwoEntities, -moveSpeed, 0.0f);
		entitiesToMove.add(&background, -(moveSpeed / 8), 0.0f);

		// Moves everything else in opposite direction of the players direction.
		entitiesToMove.move();
	}
}

void Game::getPlayerTwoKeyInputs() {
	const int MIN_Y = 70, MAX_Y = 150;

	entitiesToMove.clear();

	// Movement for player two.
	if (keystates[SDL_SCANCODE_UP]) {
		playerTwo.setTexture("resources/textures/walt_back.png");

		// Move playerTwo.
		(playerTwo.getY() == MIN_Y)
			? entitiesToMove.move(playerTwoEntities, 0.0f, 0.0f)
			: entitiesToMove.move(playerTwoEntities, 0.0f, -moveSpeed);
	}
	if (keystates[SDL_SCANCODE_LEFT]) {
		playerTwo.setTexture("resources/textures/walt_left.png");

		// Move playerTwo.
		entitiesToMove.move(playerTwoEntities, -moveSpeed, 0.0f);
	}
	if (keystates[SDL_SCANCODE_DOWN]) {
		playerTwo.setTexture("resources/textures/walt_front.png");

		// Move playerTwo.
		(playerTwo.getY() == MAX_Y)
			? entitiesToMove.move(playerTwoEntities, 0.0f, 0.0f)
			: entitiesToMove.move(playerTwoEntities, 0.0f, moveSpeed);
	}
	if (keystates[SDL_SCANCODE_RIGHT]) {
		playerTwo.setTexture("resources/textures/walt_right.png");

		// Move playerTwo.
		entitiesToMove.move(playerTwoEntities, moveSpeed, 0.0f);
	}
}

// Changes textures if the two players reach each other.
void Game::changePlayerTextures() {
	const int MIN_CLICKS = 0;

	playerDistanceX = std::abs(playerOne.getX() - playerTwo.getX());
	playerDistanceY = std::abs(playerOne.getY() - playerTwo.getY());

	if (playersAreClose()) {
		(clicks == MIN_CLICKS)
			? borrowPopUp.setTexture("resources/textures/borrow_null.png")
			: borrowPopUp.setTexture("resources/textures/borrow.png");

		if (clicks != MIN_CLICKS) {
			if (playerOne.toLeftOf(playerTwo)) {
				playerOne.setTexture("resources/textures/gus_right.png");
				playerTwo.setTexture("resources/textures/walt_left.png");
			}
			else if (playerOne.toRightOf(playerTwo)) {
				playerOne.setTexture("resources/textures/gus_left.png");
				playerTwo.setTexture("resources/textures/walt_right.png");
			}

			window.render(dialogueBox);
		}
		else {
			playerOne.setTexture("resources/textures/gus_front.png");
			playerTwo.setTexture("resources/textures/walt_front.png");
		}

		window.render(borrowPopUp);
	}
	else {
		playerOne.setTexture("resources/textures/gus_front.png");
		playerTwo.setTexture("resources/textures/walt_front.png");
	}
}