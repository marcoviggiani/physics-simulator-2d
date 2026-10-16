#include "Body.h"

void Body::integrate(const Vec2& acceleration, double dt)
{
	// Aggiorna la velocità in base all'accelerazione e al tempo
	velocity += acceleration * dt;
	// Aggiorna la posizione in base alla velocità e al tempo
	position += velocity * dt;
}
