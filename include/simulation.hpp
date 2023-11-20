#pragma once

#include <vector>

#include "world.hpp"
#include "flock.hpp"
#include "gui.hpp"

class Simulation {
private:
	//World world;

	Flock flock;
	int flockSize;

	void render();

public:
	Simulation() = delete;
	Simulation(int& _flockSize);
	~Simulation() = default;
	
	void HandleInput();
	void update();
	void run();
};


Simulation::Simulation(int& _flockSize): 
	flockSize(_flockSize), flock(_flockSize)
{
	return;
}

void Simulation::run() {
	HandleInput();
	render();
	return;
}

void Simulation::HandleInput() {
	return;
}

void Simulation::render() {
	flock.render();
	return;
}

void Simulation::update() {
	flock.update();
	return;
}