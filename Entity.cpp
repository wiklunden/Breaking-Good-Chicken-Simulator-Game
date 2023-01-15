#include "Entity.h"
#include "RenderWindow.h"

Entity::Entity()
	: pos(0, 0), texture(nullptr),
	scaler(1.0f), currentFrame{ 0, 0, 0, 0 } {}

Entity::Entity(Position p_pos, const char* texturePath, int p_w, int p_h)
	: pos(p_pos), scaler(4.0f),
	currentFrame{ 0, 0, p_w, p_h } {
	setTexture(texturePath);
}

Entity::Entity(Position p_pos, const char* texturePath, int p_w, int p_h, float p_scaler)
	: pos(p_pos), scaler(p_scaler),
	currentFrame{ 0, 0, p_w, p_h } {
	setTexture(texturePath);
}

void Entity::setTexture(const char* texturePath) {
	texture = RenderWindow::loadTexture(texturePath);
}

Position& Entity::getPos() {
	return pos;
}

float Entity::getX() const {
	return pos.x;
}

float Entity::getY() const {
	return pos.y;
}

void Entity::setPos(float p_x, float p_y) {
	pos.x = p_x;
	pos.y = p_y;
}

void Entity::move(float p_x, float p_y) {
	pos.x += p_x;
	pos.y += p_y;
}

SDL_Texture* Entity::getTexture() {
	return texture;
}

SDL_Rect Entity::getCurrentFrame() {
	return currentFrame;
}

bool Entity::offScreen() {
	bool isOffScreen = false;

	if (getX() >= 301) {
		isOffScreen = true;
		setPos(-47, getY());
	}
	if (getX() <= -48) {
		isOffScreen = true;
		setPos(300, getY());
	}

	return isOffScreen;
}

void Entity::setScaler(float p_scaler) {
	scaler = p_scaler;
}

float Entity::getScaler() const {
	return scaler;
}

std::ostream& operator<<(std::ostream& os, const Entity& e) {
	os << "x: " << e.getX() << ", y: " << e.getY();
	return os;
}

bool Entity::cmp::operator()(const Entity* e1, const Entity* e2) {
	return e1->getY() < e2->getY();
}

bool Entity::toLeftOf(Entity& e2) {
	if (getX() < e2.getX()) { return true; }
	return false;
}

bool Entity::toRightOf(Entity& e2) {
	if (getX() > e2.getX()) { return true; }
	return false;
}