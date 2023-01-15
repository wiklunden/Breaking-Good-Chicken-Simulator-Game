#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL_filesystem.h>

#include "RenderWindow.h"
#include "Entity.h"
#include "Math.h"
#include "Tiles.h"
#include "MoveEntities.h"
#include "TextureLibrary.h"
#include "RenderEntities.h"

SDL_Event event;

bool pressedKey(SDL_Keycode btn) {
	bool isPressed = false;

	if (event.type != SDL_KEYDOWN) {
		isPressed = false;
	}
	else {
		if (event.key.keysym.sym == btn) {
			isPressed = true;
		}
	}

	return isPressed;
}

int main(int argc, char* args[]) {
	// SDL initializations.
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "Initialization failed.\nError: " << SDL_GetError() << "\n";
	}
	if (!(IMG_Init(IMG_INIT_PNG))) {
		std::cout << "Could not initialize image.\nError: " << SDL_GetError() << "\n";
	}

	// Dimensions.
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const char* TITLE = "Breaking Good v1.0.0";

	// Creates window with title, dimensions and icon.
	RenderWindow window(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetWindowIcon(window.getWindow(), IMG_Load("resources/gfx/game_icon.png"));

	// Renders main menu and reads input.
	window.displayMenu();
	if (window.terminate()) { return 0; }

	// Reads presses from the keyboard.
	const Uint8* keystates = SDL_GetKeyboardState(nullptr);

	// Class responsible for automatically generating new tiles as the player moves.
	Tiles floor("resources/textures/ground.png");
	floor.create();

	const int SCREEN_CENTER_X = 126;
	const int SCREEN_CENTER_Y = 70;

	// Entity objects with a position, texture, dimensions and, if specificed, a scaler.
	Entity playerOne(Position(SCREEN_CENTER_X, SCREEN_CENTER_Y), TextureLibrary::playerOne[2], 64, 64);
	Entity playerTwo(Position(playerOne.getX() - 40, playerOne.getY() + 10), TextureLibrary::playerTwo[2], 64, 64);
	Entity dialogueBox(Position(playerTwo.getX() + 24, playerTwo.getY() - 40), "resources/textures/dialogue.png", 67, 36);
	Entity chickenPopUp(Position(SCREEN_CENTER_X, SCREEN_CENTER_Y + 60), "resources/textures/chicken.png", 64, 36);
	Entity borrowPopUp(Position(SCREEN_CENTER_X - 22, SCREEN_CENTER_Y + 60), "resources/textures/borrow.png", 104, 32);
	Entity building(Position(150, 16), "resources/textures/los_food.png", 128, 72, 6);
	Entity background(Position(-50, 0), "resources/textures/bg_new.png", 752, 288, 2);
	Entity balance(Position(2, 2), TextureLibrary::balance[0], 64, 36, 4);
	Entity quitText(Position(0, 334), "resources/textures/esc.png", 104, 32, 2);
	Entity cppIcon(Position(-10, 14), "resources/textures/cpp.png", 150, 50, 6);

	// Vectors for storing different kinds of player entities.
	std::vector<Entity*> players{ &playerOne, &playerTwo };
	std::vector<Entity*> playerTwoEntities{ &playerTwo, &dialogueBox };

	bool gameIsRunning = true, playerIsByDoor = false;
	float playerDistanceX = 0, playerDistanceY = 0;
	int clicks = 0;
	float moveSpeed = 1.0f;
	const int PLAYER_DISTANCE = 40;
	const int MIN_Y = 70, MAX_Y = 150;
	const int MIN_CLICKS = 0, MAX_CLICKS = 5;

	std::vector<Entity> wingCounter;
	Position wingOrigin(600, 0);
	Position wingPosition = wingOrigin;
	Entity wing(wingPosition, "resources/textures/wing.png", 32, 32, 2);

	while (gameIsRunning) {
		// Clears window.
		window.clear();

		// Clears underlying vector before adding in new entities to render.
		RenderEntities::clear();

		// Checks key events.
		while (SDL_PollEvent(&event)) {
			if (pressedKey(SDLK_ESCAPE)) {
				gameIsRunning = false;
				break;
			}

			// Lets playerOne borrow money from playerTwo.
			if (playerDistanceX <= PLAYER_DISTANCE && playerDistanceY <= PLAYER_DISTANCE) {
				window.render(borrowPopUp);
				if (clicks > MIN_CLICKS) {
					if (pressedKey(SDLK_e)) {
						clicks--;
						balance.setTexture(TextureLibrary::balance[clicks]);
					}
				}
			}

			// Checks if playerOne is withing bounds of the door so he can buy chicken wings.
			if (building.getX() > 54 && building.getX() <= 80 && playerOne.getY() <= 80) {
				playerIsByDoor = true;

				// Render a gray pop-up if player reaches maximum amount of clicks.
				(clicks == MAX_CLICKS)
					? chickenPopUp.setTexture("resources/textures/chicken_null.png")
					: chickenPopUp.setTexture("resources/textures/chicken.png");

				if (clicks < MAX_CLICKS) {
					if (pressedKey(SDLK_e)) {
						// Organises so there is only 10 pieces of chicken wings per row.
						if (wing.getX() <= wingOrigin.x - (32 * 9)) {
							wingPosition.x = wingOrigin.x + 32;
							wingPosition.y += 32;
						}

						// Stores chicken wing entity in vector and moves next to the left.
						wingCounter.push_back(wing);
						wingPosition.x -= 32;
						wing.setPos(wingPosition.x, wingPosition.y);

						clicks++;
						balance.setTexture(TextureLibrary::balance[clicks]);
					}
				}
			}
			else {
				playerIsByDoor = false;
			}
		}

		// Container class responsible for manipulating the position of all added entities.
		MoveEntities entitiesToMove;

		//Movement for player one.
		if (keystates[SDL_SCANCODE_W]) {
			playerOne.setTexture(TextureLibrary::playerOne[0]);

			// Move playerOne.
			(playerOne.getY() == MIN_Y)
				? playerOne.move(0.0f, 0.0f)
				: playerOne.move(0.0f, -moveSpeed);
		}
		if (keystates[SDL_SCANCODE_A]) {
			playerOne.setTexture(TextureLibrary::playerOne[1]);

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
			playerOne.setTexture(TextureLibrary::playerOne[2]);

			// Move playerOne.
			(playerOne.getY() == MAX_Y)
				? playerOne.move(0.0f, 0.0f)
				: playerOne.move(0.0f, moveSpeed);
		}
		if (keystates[SDL_SCANCODE_D]) {
			playerOne.setTexture(TextureLibrary::playerOne[3]);

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

		// MovemoveEnt for player two.
		if (keystates[SDL_SCANCODE_UP]) {
			playerTwo.setTexture(TextureLibrary::playerTwo[0]);

			// Move playerTwo.
			(playerTwo.getY() == MIN_Y)
				? entitiesToMove.move(playerTwoEntities, 0.0f, 0.0f)
				: entitiesToMove.move(playerTwoEntities, 0.0f, -moveSpeed);
		}
		if (keystates[SDL_SCANCODE_LEFT]) {
			playerTwo.setTexture(TextureLibrary::playerTwo[1]);

			// Move playerTwo.
			entitiesToMove.move(playerTwoEntities, -moveSpeed, 0.0f);
		}
		if (keystates[SDL_SCANCODE_DOWN]) {
			playerTwo.setTexture(TextureLibrary::playerTwo[2]);

			// Move playerTwo.
			(playerTwo.getY() == MAX_Y)
				? entitiesToMove.move(playerTwoEntities, 0.0f, 0.0f)
				: entitiesToMove.move(playerTwoEntities, 0.0f, moveSpeed);
		}
		if (keystates[SDL_SCANCODE_RIGHT]) {
			playerTwo.setTexture(TextureLibrary::playerTwo[3]);

			// Move playerTwo.
			entitiesToMove.move(playerTwoEntities, moveSpeed, 0.0f);
		}

		// Adds components to the underlying vector of class RenderEntities.
		RenderEntities::add(&background);
		RenderEntities::add(&cppIcon);
		RenderEntities::add(&building);
		RenderEntities::add(&balance);
		RenderEntities::add(floor.getFirstLevel());
		RenderEntities::add(floor.getSecondLevel());
		RenderEntities::add(&quitText);

		// Renders background components that are less dynamic.
		window.render(RenderEntities::get());

		// Makes players appear in order when moving up and down.
		for (auto& player : players) {
			std::sort(players.begin(), players.end(), Entity::cmp());
			window.render(*player);
		}

		// Changes textures if the two players reach each other.
		playerDistanceX = std::abs(playerOne.getX() - playerTwo.getX());
		playerDistanceY = std::abs(playerOne.getY() - playerTwo.getY());
		if (playerDistanceX <= PLAYER_DISTANCE && playerDistanceY <= PLAYER_DISTANCE) {
			(clicks == MIN_CLICKS)
				? borrowPopUp.setTexture("resources/textures/borrow_null.png")
				: borrowPopUp.setTexture("resources/textures/borrow.png");

			if (clicks != MIN_CLICKS) {
				if (playerOne.getX() < playerTwo.getX()) {
					playerOne.setTexture(TextureLibrary::playerOne[3]);
					playerTwo.setTexture(TextureLibrary::playerTwo[1]);
				}
				else if (playerOne.getX() > playerTwo.getX()) {
					playerOne.setTexture(TextureLibrary::playerOne[1]);
					playerTwo.setTexture(TextureLibrary::playerTwo[3]);
				}

				window.render(dialogueBox);
			}
			else {
				playerOne.setTexture(TextureLibrary::playerOne[2]);
				playerTwo.setTexture(TextureLibrary::playerTwo[2]);
			}

			window.render(borrowPopUp);
		}
		else {
			playerOne.setTexture(TextureLibrary::playerOne[2]);
			playerTwo.setTexture(TextureLibrary::playerTwo[2]);
		}

		if (playerIsByDoor) {
			window.render(chickenPopUp);
		}

		for (auto& w : wingCounter) {
			window.render(w);
		}

		/*window.render(quitText);*/

		// Displays all entities.
		window.display();
	}

	// Destroys and cleans the window.
	window.cleanUp();

	// Quits SDL.
	SDL_Quit();

	return 0;
}