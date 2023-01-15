#include "MoveEntities.h"

MoveEntities::tmpEntity::tmpEntity()
	: x(0.0f), y(0.0f), obj() {}

void MoveEntities::add(Entity* p_entity, float p_x, float p_y) {
	tmp.obj = p_entity;
	tmp.x = p_x;
	tmp.y = p_y;

	vec.push_back(tmp);
}

void MoveEntities::add(std::vector<Entity*>& eVec, float p_x, float p_y) {
	tmp.x = p_x;
	tmp.y = p_y;

	for (auto& e : eVec) {
		tmp.obj = e;
		vec.push_back(tmp);
	}
}

void MoveEntities::add(std::deque<Entity>& eDeq, float p_x, float p_y) {
	tmp.x = p_x;
	tmp.y = p_y;

	for (auto& e : eDeq) {
		tmp.obj = &e;
		vec.push_back(tmp);
	}
}

void MoveEntities::move() {
	for (auto& it : vec) {
		it.obj->move(it.x, it.y);
	}
}

void MoveEntities::move(std::vector<Entity*>& p_container, float p_x, float p_y) {
	for (auto& it : p_container) {
		it->move(p_x, p_y);
	}
}

void MoveEntities::autoMove(std::vector<Entity*>& p_container) {
	int moveOrNot = rand() % 100;
	if (moveOrNot == 0) {
		int pAxis = rand() % 2;
		float autoSpeed = 5.0f;
		if (pAxis == 0) {
			int leftOrRight = rand() % 2;
			if (leftOrRight == 0) {
				//playerTwo.setTexture("resources/textures/walt_left.png");
				move(p_container, -autoSpeed, 0.0f);
			}
			else {
				//playerTwo.setTexture("resources/textures/walt_right.png");
				move(p_container, autoSpeed, 0.0f);
			}
		}
		else {
			int leftOrRight = rand() % 2;
			if (leftOrRight == 0) {
				//playerTwo.setTexture("resources/textures/walt_back.png");
				move(p_container, 0.0f, -autoSpeed);
			}
			else {
				//playerTwo.setTexture("resources/textures/walt_front.png");
				move(p_container, 0.0f, autoSpeed);
			}
		}
	}
}

bool MoveEntities::empty() {
	return (vec.size() == 0);
}

void MoveEntities::clear() {
	for (auto& it : vec) {
		vec.pop_back();
	}
}