#include "Collision.h"

Collision::Collision(glm::vec2 mtv)
{
	Normal = glm::normalize(mtv);
	MinimumTranslationVector = mtv;
	Collided = pow(mtv.x, 2) + pow(mtv.y, 2) != 0;
}
Collision::Collision(glm::vec2 mtv, glm::vec2 normal)
{
	Normal = mtv;
	MinimumTranslationVector = mtv;
	Collided = pow(mtv.x, 2) + pow(mtv.y, 2) != 0;
}