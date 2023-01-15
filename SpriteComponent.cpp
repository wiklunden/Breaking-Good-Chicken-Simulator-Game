#include "SpriteComponent.h"
#include "RenderWindow.h"

SpriteComponent::SpriteComponent()
	: texture(nullptr) {}

SpriteComponent::SpriteComponent(const char* FILE_PATH) {
	texture = RenderWindow::loadTexture(FILE_PATH);
}

SpriteComponent::~SpriteComponent() {
	SDL_DestroyTexture(texture);
}

SDL_Texture* SpriteComponent::getTexture() const {
	return texture;
}