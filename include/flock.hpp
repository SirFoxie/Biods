#pragma once

#include "./common/common.hpp"

#include "world.hpp"
#include "boid.hpp"

class Flock{
public:


	Flock() = default;
	Flock(int _size, World& _map);
	~Flock() = default;

	void addTarget(const Vector2& target, const int BiodID);
	std::vector<Boid>& getBoids() {
		return Boids;
	}

	int size() { return Boids.size(); }
	Boid::Weight& weights() { return BoidWeights; }

	void update();
	void render();

private:
	std::vector<Boid> Boids;
	World& map;

	Boid::Weight BoidWeights = { 1.0, 1.0, 1.5 };

	void debug();
};


Flock::Flock(int _size, World& _map) : 
	Boids(_size), map(_map)
{
	return;
}

void Flock::addTarget(const Vector2& target, const int BiodID) {
	
	return;
}

void Flock::update() {

	for (Boid& boid : Boids) {
		boid.update(Boids, BoidWeights);
	}
	return;
}

void Flock::render() {
	
	for (Boid& boid : Boids) {
		boid.render();
	}
	debug();
	return;
}

void Flock::debug() {
	DrawText(TextFormat("FLOCK SIZE: %d", this->Boids.size()), 0, 20, 10, BLUE);
}

