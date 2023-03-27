#include "Game.h"

int Game::run() {
	// SDL initializations.
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "Initialization failed.\nError: " << SDL_GetError() << "\n";
	}
	if (!(IMG_Init(IMG_INIT_PNG))) {
		std::cout << "Could not initialize image.\nError: " << SDL_GetError() << "\n";
	}

	// Reads presses from the keyboard.
	const Uint8* keystates = SDL_GetKeyboardState(nullptr);

	// Class responsible for automatically generating new tiles as the player moves.
	Tiles floor("resources/textures/ground.png");
	floor.create();

	SDL_SetWindowIcon(window.getWindow(), IMG_Load("resources/gfx/game_icon.png"));

	// Renders main menu and reads input.
	window.displayMenu();
	if (window.terminate()) {
		return 1;
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
	wing = Entity(wingPosition, "resoucres/textures/wing.png", 32, 32, 2);
}