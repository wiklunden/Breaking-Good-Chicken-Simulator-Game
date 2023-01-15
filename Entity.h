#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Math.h"

class Entity {
public:
	Entity();
	Entity(Position, const char*, int, int); // Copy constructor without scaler
	Entity(Position, const char*, int, int, float); // Copy constructor with scaler
	void setTexture(const char*);
	Position& getPos();
	float getX() const;
	float getY() const;
	void setPos(float, float);
	void move(float, float);
	SDL_Texture* getTexture();
	SDL_Rect getCurrentFrame();
	bool offScreen();
	void setScaler(float);
	float getScaler() const;
	bool toLeftOf(Entity&);
	bool toRightOf(Entity&);

	friend std::ostream& operator<<(std::ostream&, const Entity&);
	struct cmp {
		bool operator()(const Entity*, const Entity*);
	};

private:
	Position pos;
	SDL_Rect currentFrame;
	SDL_Texture* texture;
	float scaler;
};