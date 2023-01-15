#pragma once

#include <deque>
#include <SDL.h>
#include "Entity.h"
#include "RenderWindow.h"

class Tiles {
public:
	explicit Tiles(const char*);
	void create();
	std::deque<Entity>& getFirstLevel();
	std::deque<Entity>& getSecondLevel();

private:
	void autogenerate(std::deque<Entity>&);
	const char* texturePath;
	SDL_Texture* texture;
	std::deque<Entity> firstLevel, secondLevel;
	int tileSize;
};