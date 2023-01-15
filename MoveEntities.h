#pragma once

#include <vector>
#include <deque>
#include "Entity.h"

class MoveEntities {
public:
	void add(Entity*, float, float);
	void add(std::vector<Entity*>&, float, float);
	void add(std::deque<Entity>&, float, float);
	void move();
	void move(std::vector<Entity*>&, float, float);
	void autoMove(std::vector<Entity*>&);
	bool empty();
	void clear();

private:
	struct tmpEntity {
		tmpEntity();
		Entity* obj;
		float x, y;
	};
	tmpEntity tmp;
	std::vector<tmpEntity> vec;
};