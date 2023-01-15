#include "Tiles.h"

Tiles::Tiles(const char* p_texturePath)
	: texturePath(p_texturePath), texture(RenderWindow::loadTexture(texturePath)),
	firstLevel(1), secondLevel(1), tileSize(32) {}

// Sets base tiles which are used for automatically generating new ones.
void Tiles::create() {
	const int GROUND_AXIS_Y = 116;

	for (int i = -(tileSize * 3); i < -(tileSize * 2); i += tileSize) {
		// Sets first level at normal height.
		firstLevel.push_back(Entity(Position(i, GROUND_AXIS_Y),
			texturePath, tileSize, tileSize));

		// Sets second level one tile size (32 px) below first level.
		secondLevel.push_back(Entity(Position(i, GROUND_AXIS_Y + tileSize),
			texturePath, tileSize, tileSize));
	}
}

// Returns the deque containing all floor tiles.
std::deque<Entity>& Tiles::getFirstLevel() {
	autogenerate(firstLevel);
	return firstLevel;
}

std::deque<Entity>& Tiles::getSecondLevel() {
	autogenerate(secondLevel);
	return secondLevel;
}

void Tiles::autogenerate(std::deque<Entity>& queue) {
	const Entity* firstTile = &queue.front();
	const Entity* lastTile = &queue.back();

	// Bug fix. Generated new tiles at (0, 0).
	// Moves first tile to same Y-level as other tiles.
	if (firstTile->getY() == 0) {
		queue.front().move(0.0f, queue.back().getY());
	}

	// Places a new tile at the front of queue.
	if (firstTile->getX() >= -20) {
		queue.push_front(Entity(
			Position(queue.front().getX() - tileSize,
			queue.front().getY()),
			texturePath, tileSize, tileSize)
		);
	}

	// Places a new tile at the back of queue.
	if (lastTile->getX() <= 295) {
		queue.push_back(Entity(
			Position(queue.back().getX() + tileSize,
			queue.back().getY()),
			texturePath, tileSize, tileSize)
		);
	}

	if (firstTile->getX() <= -35) {
		queue.pop_front();
	}

	if (lastTile->getX() >= 330) {
		queue.pop_back();
	}
}