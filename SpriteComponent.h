#pragma once

#include <SDL.h>
#include <vector>

class SpriteComponent {
public:
	SpriteComponent();
	explicit SpriteComponent(const char*);
	~SpriteComponent();
	SDL_Texture* getTexture() const;

private:
	SDL_Texture* texture;
};

