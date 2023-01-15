#include "RenderEntities.h"

std::vector<Entity*> RenderEntities::container(0);

void RenderEntities::add(Entity* e) {
	container.push_back(e);
}

void RenderEntities::add(std::vector<Entity*>& vec) {
	for (auto& e : vec) {
		container.push_back(e);
	}
}

void RenderEntities::add(std::deque<Entity>& vec) {
	for (auto& e : vec) {
		container.push_back(&e);
	}
}

void RenderEntities::render() {
	for (auto& e : container) {
		RenderWindow::render(*e);
	}
}

void RenderEntities::render(Entity& e) {
	RenderWindow::render(e);
}

void RenderEntities::clear() {
	for (auto& e : container) {
		container.pop_back();
	}
}

std::vector<Entity*>& RenderEntities::get() {
	return container;
}