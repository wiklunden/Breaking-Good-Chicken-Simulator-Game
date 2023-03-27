#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_filesystem.h>

#include "RenderWindow.h"
#include "Entity.h"
#include "Math.h"
#include "Tiles.h"
#include "MoveEntities.h"
#include "RenderEntities.h"

class Game {
public:
	Game() :
		WINDOW_WIDTH(1280),
		WINDOW_HEIGHT(720),
		GAME_TITLE("Breaking Good v.1.0.0"),
		window(GAME_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT),
		floor("resources/textures/ground.png"),
		gameIsRunning(false),
		keystates(SDL_GetKeyboardState(nullptr)),
		clicks(0),
		moveSpeed(1.0f),
		playerDistanceX(0),
		playerDistanceY(0),
		SCREEN_CENTER_X(126),
		SCREEN_CENTER_Y(70)
	{}
	void run();
private:
	void stop();
	void pause();
	void handleEvents();
	void update();
	void initiateEntities();
	void getPlayerOneKeyInputs();
	void getPlayerTwoKeyInputs();
	void renderUIElements();
	void changePlayerTextures();
	void renderChickenWings();
	bool pressedKey(const SDL_Keycode);
	bool playersAreClose();
	bool playerOneByDoor();
	int displayMainMenu();

	// Reads presses from the keyboard.
	const Uint8* keystates;

	const int WINDOW_WIDTH, WINDOW_HEIGHT;
	const char* GAME_TITLE;
	RenderWindow window;

	Tiles floor;
	bool gameIsRunning;

	// Container class responsible for manipulating the position of all added entities.
	MoveEntities entitiesToMove;

	Entity playerOne,
		playerTwo,
		dialogueBox,
		chickenPopUp,
		borrowPopUp,
		building,
		background,
		balance,
		quitText,
		cppIcon,
		wing;

	Position wingOrigin{ 600, 0 };
	Position wingPosition = wingOrigin;

	std::vector<Entity*> players, playerTwoEntities;
	std::vector<Entity> wingCounter;

	SDL_Event event{};

	float playerDistanceX, playerDistanceY, moveSpeed;
	int clicks;

	const int SCREEN_CENTER_X, SCREEN_CENTER_Y;
	const char* balanceTextures[6]{
		"resources/textures/balance/10.png",
		"resources/textures/balance/8.png",
		"resources/textures/balance/6.png",
		"resources/textures/balance/4.png",
		"resources/textures/balance/2.png",
		"resources/textures/balance/0.png"
	};
};