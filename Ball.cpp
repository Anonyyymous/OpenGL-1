#include "Ball.h"

BallObject::BallObject() : GameObject(), Radius(12.5f), Stuck(true) {}
BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite) : GameObject(pos, glm::vec2(radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true) {}
void Inverse(float* f) {
	*f = -(*f);
}
glm::vec2 BallObject::Move(float dt, unsigned int window_width) {
	if (!Stuck) {
		Position += Velocity * dt;

		// check bounds
		if (Position.x <= 0.0f) {
			// Velocity.x = -Velocity.x;
			Inverse(&Velocity.x);
			Position.x = 0;
		} else if (Position.x + Size.x >= window_width) {
			// Velocity.x = -Velocity.x;
			Inverse(&Velocity.x);
			Position.x = window_width - Size.x;
		}

		if (Position.y <= 0.0f) {
			Inverse(&Velocity.y);
			// Velocity.y = -Velocity.y;
			Position.y = 0;
		} // if position.y is too large then we lose, so this code is somewhere else
	}
	return Position;
}
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity) {
	Position = position;
	Velocity = velocity;
	Stuck = true;
}
