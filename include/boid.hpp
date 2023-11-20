#pragma once

#include "raylib.h"

#include "./common/common.hpp"
#include "world.hpp"

#define MIN_VEL -5
#define MAX_VEL 5

class Boid {
public:
	Boid();
	~Boid();

	struct Weight {
		float alignment;
		float cohesion;
		float seperation;
	};

	void update(const std::vector <Boid>& boids, const Weight& boidWeights);
	void render();
	void addTarget(const Vector2& newTarget);
private:
	int BoidId = 0;
	static int BoidCount;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	Vector2 alignment(const std::vector <Boid>& boids);
	Vector2 cohesion(const std::vector <Boid>& boids);
	Vector2 separation(const std::vector <Boid>& boids);

	void avoid();
	enum BoidState { STANDBY, ROAMING, SEEKING } state;

	Vector2 seek(const Vector2& target);
	void edge();

	void debug();
};

int Boid::BoidCount = 0;

Boid::Boid() {
	this->BoidId = BoidCount++;
	World::Area StartArea = GridPosToPixelPos((World::getInstance().getStartArea()));
	this->position = { (float)GetRandomValue(StartArea.x, StartArea.width),
						(float)GetRandomValue(StartArea.y, StartArea.height) };
	this->velocity = { (float)GetRandomValue(MIN_VEL, MAX_VEL), (float)GetRandomValue(MIN_VEL, MAX_VEL) };
	this->acceleration = { 1.0, 1.0 };
	this->state = ROAMING;
	return;
}

Boid::~Boid() {

}


void Boid::update(const std::vector <Boid>& boids, const Weight& boidWeights) {
	Vector2 aligmentForce = alignment(boids);
	Vector2 cohesionForce = cohesion(boids);
	Vector2 separationForce = separation(boids);

	this->acceleration = Vector2Add(acceleration, Vector2Scale(aligmentForce, boidWeights.alignment));
	this->acceleration = Vector2Add(acceleration, Vector2Scale(cohesionForce, boidWeights.cohesion));
	this->acceleration = Vector2Add(acceleration, Vector2Scale(separationForce, boidWeights.seperation));

	switch (state) {
	case STANDBY:
		return;
		break;
	case ROAMING:
		break;
	case SEEKING:
		//seek();
		break;
	}
	this->acceleration = Vector2Scale(acceleration, 0.4);
	this->velocity = Vector2Add(velocity, acceleration);
	this->velocity = Vector2ClampValue(velocity, MIN_VEL, MAX_VEL);
	this->position = Vector2Add(position, velocity);
	this->acceleration = Vector2Scale(acceleration, 0);

	edge();
	return;
}

Vector2 Boid::alignment(const std::vector <Boid>& boids) {
	
	float perception = 25; // Field of vision
	Vector2 steering = { 0.0, 0.0 }; //Steering force
	int totalSurroundingBoids = 0;


	for (const Boid& otherboid : boids) {
		if (&otherboid != this) {
			float distance = Vector2Distance(this->position, otherboid.position);
			if ((distance > 0) && (distance < perception)) {
				steering = Vector2Add(steering, otherboid.velocity);
				totalSurroundingBoids++;
			}
		}
	}
	if (totalSurroundingBoids > 0) {
		steering = Vector2Scale(steering, 1.0/(float)totalSurroundingBoids);
		steering = Vector2Scale(steering, 2);
		steering = Vector2Subtract(steering, this->velocity);
		steering = Vector2ClampValue(steering, -0.2, 0.2);
	}
	return steering;
}

Vector2 Boid::cohesion(const std::vector <Boid>& boids) {

	float perception = 50;
	Vector2 steering = { 0.0, 0.0 };
	int totalSurroundingBoids = 0;

	for (const Boid& otherboid : boids) {
		if (&otherboid != this) {
			float distance = Vector2Distance(this->position, otherboid.position);
			if ((distance > 0) && (distance < perception)) {
				steering = Vector2Add(steering, otherboid.position);
				totalSurroundingBoids++;
			}
		}
	}
	if (totalSurroundingBoids > 0) {
		steering = Vector2Scale(steering, 1.0 / (float)totalSurroundingBoids);
		steering = seek(steering);
	}

	return steering;
}

Vector2 Boid::seek(const Vector2& target) {
	Vector2 steering = { 0.0, 0.0 };
	Vector2 distance = Vector2Subtract(target, position);
	
	steering = Vector2Subtract(distance, velocity);
	steering = Vector2ClampValue(steering, -0.2, 0.2);
	return steering;
}


Vector2 Boid::separation(const std::vector <Boid>& boids) {
	Vector2 steering = { 0.0, 0.0 };
	float perception = 24;
	int totalSurroundingBoids = 0;

	for (const Boid& otherboid : boids) {
		if (&otherboid != this) {
			float distance = Vector2Distance(this->position, otherboid.position);
			if ((distance > 0) && (distance < perception)) {
				Vector2 diff = Vector2Subtract(this->position, otherboid.position);
				diff = Vector2Scale(diff, distance * distance);
				steering = Vector2Add(steering, diff);
				totalSurroundingBoids++;
			}
		}
	}
	if (totalSurroundingBoids > 0) {
		steering = Vector2Scale(steering, 1.0 / (float)totalSurroundingBoids);
		steering = Vector2Scale(steering, 2);
		steering = Vector2Subtract(steering, this->velocity);
		steering = Vector2ClampValue(steering, -0.2, 0.2);
	}
	return steering;
}

void Boid::edge() {
	World::Area borders = GridPosToPixelPos(World::getInstance().getMap());
	if (position.x < borders.x) position.x += borders.width;
	if (position.x > borders.width) position.x -= borders.width;
	if (position.y < borders.y) position.y += borders.height;
	if (position.y > borders.height) position.y -= borders.height;
}

void Boid::render() {
	DrawCircle(position.x , position.y , 5, GREEN);
	//debug();
	return;
}

void Boid::debug() {
	DrawText(TextFormat("ID: %i", this->BoidId), position.x, position.y - 10, 10, LIME);
	DrawText(TextFormat("[%.2f, %.2f]", position.x, position.y), position.x, position.y - 5, 10, LIME);
	DrawText(TextFormat("[%.2f, %.2f]", velocity.x, velocity.y), position.x, position.y + 5, 10, LIME);
}

