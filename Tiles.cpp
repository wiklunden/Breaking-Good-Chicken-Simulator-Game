#include "Tiles.h"

Tiles::Tiles(const char* p_texturePath)
	: texturePath(p_texturePath), texture(RenderWindow::loadTexture(texturePath)),
	firstLevel(1), secondLevel(1), tileSize(32) {}

// Sets base tiles which are used for automatically generating new ones.
void Tiles::create() {
	const int GROUND_AXIS_Y = 116;

	for (int i = -(tileSize * 3); i < -(tileSize * 2); i += tileSize) {
		firstLevel.push_back(Entity(Position(i, GROUND_AXIS_Y),
			texturePath, tileSize, tileSize));

		secondLevel.push_back(Entity(Position(i, GROUND_AXIS_Y + tileSize),
			texturePath, tileSize, tileSize));
	}
}

// Returns the deque containing all floor tiles.
std::deque<Entity>& Tiles::getFirstLevel() {
	checkBounds(firstLevel);
	return firstLevel;
}
std::deque<Entity>& Tiles::getSecondLevel() {
	checkBounds(secondLevel);
	return secondLevel;
}

void Tiles::checkBounds(std::deque<Entity>& queue) {
	// Bug fix. Generated new tiles at x:0 and y:0.
	if (queue.front().getY() == 0) {
		queue.front().move(0.0f, queue.back().getY());
	}

	if (queue.front().getX() >= -20) {
		queue.push_front(Entity(
			Position(queue.front().getX() - tileSize, queue.front().getY()),
			texturePath, tileSize, tileSize));
	}

	if (queue.back().getX() <= 295) {
		queue.push_back(Entity(
			Position(queue.back().getX() + tileSize, queue.back().getY()),
			texturePath, tileSize, tileSize));
	}

	if (queue.front().getX() <= -35) {
		queue.pop_front();
	}

	if (queue.back().getX() >= 330) {
		queue.pop_back();
	}
}