#pragma once
#include "Vec2.h"

struct Body
{
	Vec2 position;
	Vec2 velocity;
	double mass;
	double radius;
	double restitution = 0.8; // Coefficient of restitution (bounciness)

	void integrate(const Vec2& acceleration, double dt);
};



