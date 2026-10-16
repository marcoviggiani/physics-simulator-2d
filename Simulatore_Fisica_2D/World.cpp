#include "World.h"
#include <algorithm>
#include <stdexcept>

void World::step(double dt) {
	for (Body& body : bodies) {
		body.integrate(gravity, dt);
	}
	checkCollisions();
	for (Body& body : bodies) {
		collideWalls(body);
	}
}

void World::collideWalls(Body& body) {
	if (body.position.y - body.radius < 0) {
		body.position.y = body.radius;
		body.velocity.y = -body.velocity.y * body.restitution; // coefficiente di restituzione
	}
	if (body.position.x - body.radius < 0) {
		body.position.x = body.radius;
		body.velocity.x = -body.velocity.x * body.restitution; // coefficiente di restituzione
	}
	if (body.position.x + body.radius > width) {
		body.position.x = width - body.radius;
		body.velocity.x = -body.velocity.x * body.restitution; // coefficiente di restituzione
	}
	if (body.position.y + body.radius > height) {
		body.position.y = height - body.radius;
		body.velocity.y = -body.velocity.y * body.restitution; // coefficiente di restituzione
	}
}

void World::checkCollisions() {
	for (size_t i = 0; i < bodies.size(); i++) {
		for (size_t j = i + 1; j < bodies.size(); j++) {
			// Controllo delle collisioni tra i corpi
			Body& a = bodies[i];
			Body& b = bodies[j];
			double minDistance = a.radius + b.radius;
			Vec2 delta = b.position - a.position;
			double distanceSquared = delta.lengthSquared();

			if (distanceSquared == 0) continue; // Corpi sono sovrapposti esattamente, non possiamo calcolare la normale

			if (distanceSquared >= minDistance * minDistance) continue; // Non c'è collisione

			double distance = delta.length();
			Vec2 n = delta / distance; // Direzione della normale (vettore da "a" a "b") tra i due corpi

			double overlap = minDistance - distance;

			double relativeVelocity = dot(a.velocity - b.velocity, n);

			a.position -= (overlap * b.mass / (a.mass + b.mass)) * n;
			b.position += (overlap * a.mass / (a.mass + b.mass)) * n;


			if (relativeVelocity <= 0) continue; // I corpi si stanno allontanando, non c'è collisione

			double e = std::min(a.restitution, b.restitution); // Coefficiente di restituzione (bounciness)

			double J = (1 + e) * relativeVelocity / (1 / a.mass + 1 / b.mass); // Impulso della collisione

			Vec2 impulse = J * n;

			a.velocity -= impulse / a.mass;
			b.velocity += impulse / b.mass;
		}
	}
}

void World::addBody(const Body& body) {
	if (!(body.radius > 0 && body.mass > 0 && body.restitution >= 0 && body.restitution <= 1)) {
		throw std::invalid_argument("...");
	}
	bodies.push_back(body);
}
