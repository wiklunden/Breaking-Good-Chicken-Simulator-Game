#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

struct TextureLibrary {
public:
	static const char** getSet(int);
	static const char* balance[];
	static const char* at(size_t);
	static const char* playerOne[];
	static const char* playerTwo[];
	/*static const char* jesse[];
	static const char* hank[];*/

private:
	static int index;
	static const int ARR_SIZE = 5;
	static const char** textureArr[];
};