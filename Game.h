#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

#include "RenderWindow.h"
#include "Tiles.h"

class Game {
public:
	Game() :
		window(GAME_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT),
		floor("resources/textures/ground.png"),
		gameIsRunning(false),
		clicks(0),
		SCREEN_CENTER_X(126),
		SCREEN_CENTER_Y(70)
	{}
	int run();
private:
	void handleEvents();
	void update();
	void render();
	void initiateEntities();

	RenderWindow window;
	Tiles floor;
	Position wingOrigin{ 600, 0 };
	Position wingPosition = wingOrigin;

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

	std::vector<Entity*> players, playerTwoEntites;
	std::vector<Entity> wingCounter;

	SDL_Event event;

	bool gameIsRunning;
	float playerDistanceX = 0, playerDistanceY = 0;
	float moveSpeed = 1.0f;
	int clicks;

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const char* GAME_TITLE = "Breaking Good v.1.0.0";
	const int PLAYER_DISTANCE = 40;
	const int MIN_Y = 70, MAX_Y = 150;
	const int MIN_CLICKS = 0, MAX_CLICKS = 5;
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