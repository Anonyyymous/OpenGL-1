#ifndef COLLISION_H
#define COLLISION_H

#include<glad/glad.h>
#include<glm/glm.hpp>

class Collision
{
public:
	glm::vec2 MinimumTranslationVector;
	glm::vec2 Normal;
	bool Collided;
	Collision(glm::vec2 mtv);
	Collision(glm::vec2 mtv, glm::vec2 normal);
};


#endif