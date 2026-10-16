#pragma once
#include "Body.h"
#include "Vec2.h"
#include <vector>


struct World
{
	std::vector<Body> bodies;
	Vec2 gravity;
	double width = 20.0, height = 20.0;

	void step(double dt);
	void collideWalls(Body& body);
	void checkCollisions();
	void addBody(const Body& body);
};

