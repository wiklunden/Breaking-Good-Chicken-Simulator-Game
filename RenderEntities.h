#pragma once

#include <SDL.h>
#include <vector>
#include <deque>
#include "RenderWindow.h"
#include "Entity.h"

class RenderEntities {
public:
	RenderEntities() = default;
	static void add(Entity*);
	static void add(std::vector<Entity*>&);
	static void add(std::deque<Entity>&);
	static void render();
	static void render(Entity&);
	static void clear();
	static std::vector<Entity*>& get();

private:
	static std::vector<Entity*> container;
};