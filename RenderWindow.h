#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "Entity.h"

class RenderWindow {
public:
	RenderWindow() = default;
	RenderWindow(const char*, const int&, const int&);
	SDL_Window* getWindow() const;
	static SDL_Texture* loadTexture(const char*);
	int getRefreshRate();
	void cleanUp();
	void clear();
	static void render(Entity&);
	static void render(std::vector<Entity*>&);
	void display();

private:
	SDL_Window* window;
	// OpenGL renderer for displaying things.
	static SDL_Renderer* renderer;
};

