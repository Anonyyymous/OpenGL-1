#ifndef TRIANGLE_H
#define TRIANGLE_H

#include"Collision.h"

// indices allocation should follow left hand rule

class Triangle
{
private:
	glm::vec2 _normals[3];
	glm::vec2 _points[3];
	glm::vec2 TangentToNormal(glm::vec2 point1, glm::vec2 point2);

public:
	glm::vec2 _centre; // rename
	bool colliding = false;
	Triangle(glm::vec2 point1, glm::vec2 point2, glm::vec2 point3);

	Collision BallCollidesWithTriangle(glm::vec2 ball_position, float radius);
	bool TestPosition(glm::vec2 pos);
};

#endif