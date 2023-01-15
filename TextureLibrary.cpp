#include "TextureLibrary.h"

int TextureLibrary::index = 0;

const char* TextureLibrary::balance[6]{
	"resources/textures/balance/10.png",
	"resources/textures/balance/8.png",
	"resources/textures/balance/6.png",
	"resources/textures/balance/4.png",
	"resources/textures/balance/2.png",
	"resources/textures/balance/0.png"
};

const char* TextureLibrary::playerOne[ARR_SIZE]{
	"resources/textures/gus_back.png",
	"resources/textures/gus_left.png",
	"resources/textures/gus_front.png",
	"resources/textures/gus_right.png",
	"resources/textures/gus_eat_front.png"
};

const char* TextureLibrary::playerTwo[ARR_SIZE]{
	"resources/textures/walt_back.png",
	"resources/textures/walt_left.png",
	"resources/textures/walt_front.png",
	"resources/textures/walt_right.png"
};

//const char* TextureLibrary::jesse[ARR_SIZE]{
//	"resources/textures/jesse_back.png",
//	"resources/textures/jesse_left.png",
//	"resources/textures/jesse_front.png",
//	"resources/textures/jesse_right.png"
//};
//
//const char* TextureLibrary::hank[ARR_SIZE]{
//	"resources/textures/hank_back.png",
//	"resources/textures/hank_left.png",
//	"resources/textures/hank_front.png",
//	"resources/textures/hank_right.png"
//};

const char** TextureLibrary::textureArr[4]{
	playerOne, playerTwo
};

const char** TextureLibrary::getSet(int n) {
	index = n;
	return textureArr[n];
}

const char* TextureLibrary::at(size_t n) {
	return TextureLibrary::getSet(index)[n];
}