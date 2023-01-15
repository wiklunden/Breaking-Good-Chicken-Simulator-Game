#pragma once

#include <iostream>

struct Position {
public:
	float x, y;

	Position()
		: x(0), y(0) {}

	Position(float p_x, float p_y)
		: x(p_x), y(p_y) {}

	void print() const {
		std::cout << x << ", " << y << "\n";
	}
};