// Simulatore_Fisica_2D.cpp: definisce il punto di ingresso dell'applicazione.
//

#include "Simulatore_Fisica_2D.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include "Vec2.h"
#include "Body.h"
#include "World.h"

// Energia totale = cinetica + potenziale (calcolata a mano, senza metodi)
double totalEnergy(const World& world) {
	double energy = 0.0;
	for (const Body& b : world.bodies) {
		double v2 = b.velocity.x * b.velocity.x + b.velocity.y * b.velocity.y;
		double kinetic = 0.5 * b.mass * v2;
		double potential = b.mass * (-world.gravity.y) * b.position.y;
		energy += kinetic + potential;
	}
	return energy;
}

// Velocità massima tra tutte le palline
double maxSpeed(const World& world) {
	double best = 0.0;
	for (const Body& b : world.bodies) {
		double speed = std::sqrt(b.velocity.x * b.velocity.x + b.velocity.y * b.velocity.y);
		if (speed > best) best = speed;
	}
	return best;
}

// Massima compenetrazione tra due palline (0 = nessuna)
double maxOverlap(const World& world) {
	double worst = 0.0;
	for (size_t i = 0; i < world.bodies.size(); i++) {
		for (size_t j = i + 1; j < world.bodies.size(); j++) {
			const Body& a = world.bodies[i];
			const Body& b = world.bodies[j];
			double dx = b.position.x - a.position.x;
			double dy = b.position.y - a.position.y;
			double overlap = (a.radius + b.radius) - std::sqrt(dx * dx + dy * dy);
			if (overlap > worst) worst = overlap;
		}
	}
	return worst;
}

// Quante palline sono (anche solo in parte) fuori dalla scatola
int ballsOutside(const World& world) {
	int count = 0;
	const double tolerance = 1e-9;
	for (const Body& b : world.bodies) {
		bool outside = b.position.x - b.radius < -tolerance
			|| b.position.x + b.radius > world.width + tolerance
			|| b.position.y - b.radius < -tolerance
			|| b.position.y + b.radius > world.height + tolerance;
		if (outside) count++;
	}
	return count;
}

int main()
{
	const double dt = 0.01;          // passo di integrazione
	const int steps = 3000;          // 30 secondi simulati
	const int printEvery = 250;      // stampa ogni 250 passi
	const double restitution = 0.9;

	World world;
	world.gravity = Vec2{ 0, -9.81 };

	// Generatore casuale con seme fisso: stessi numeri a ogni esecuzione
	std::mt19937 rng(42);
	auto random01 = [&rng]() { return (rng() % 10000) / 10000.0; };

	// 20 palline in una griglia di 5 colonne x 4 righe (nessuna sovrapposta all'inizio)
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 5; col++) {
			double radius = 0.4 + 0.3 * random01();      // raggio tra 0.4 e 0.7
			double mass = 3.14159 * radius * radius;     // massa proporzionale all'area
			Vec2 position{ 2.5 + col * 3.75, 6.0 + row * 3.5 };
			Vec2 velocity{ (random01() - 0.5) * 10.0, (random01() - 0.5) * 10.0 };
			Body body{ position, velocity, mass, radius, restitution };
			Body invalidBody{ Vec2{0, 0}, Vec2{0, 0}, -1.0, radius, restitution }; // Corpo con massa negativa per test
			try {
				world.addBody(body);
				world.addBody(invalidBody); // Provo ad aggiungere un corpo con massa negativa
			}
			catch (const std::exception& e) {
				std::cerr << "Errore nell'aggiunta della pallina: " << e.what() << "\n";
			}
		}
	}

	const double initialEnergy = totalEnergy(world);

	std::cout << std::fixed << std::setprecision(3);
	std::cout << "Palline: " << world.bodies.size() << ", restitution: " << restitution << "\n";
	std::cout << "step | energia/E0 | vel. max | sovrapp. max | fuori scatola\n";

	for (int i = 0; i <= steps; i++) {
		if (i % printEvery == 0) {
			std::cout << std::setw(4) << i << " | "
				<< std::setw(10) << totalEnergy(world) / initialEnergy << " | "
				<< std::setw(8) << maxSpeed(world) << " | "
				<< std::setw(12) << maxOverlap(world) << " | "
				<< ballsOutside(world) << "\n";
		}
		world.step(dt);
	}

	return 0;
}