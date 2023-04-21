/*
	Name:			Milton Wiklund.
	Description:	8-bit-like game featuring characters from the Netflix show Breaking Bad.
					This game is not copyrighted nor generating revenue in any form.
					This game was created as a final project for a course called 'Programming methodology' (C++),
					under the program 'Datateknik' at Mid Sweden University.

					The game is run by calling the "game engine"'s run()-function.
					With usage of the SDL2-library, the game is being displayed in a 1280x720 window along with textures and movement.
	Creation date:	21/10-2022.
*/

#include <iostream>
#include "Game.h"

int main(int argc, char* args[]) {
	Game game;
	game.run();

	return 0;
}
