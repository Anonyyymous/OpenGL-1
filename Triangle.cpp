#include "Triangle.h"

glm::vec2 Rotate90(glm::vec2 target)
{
	return glm::vec2(-target.y, target.x);
}
glm::vec2 Midpoint(glm::vec2 v1, glm::vec2 v2)
{
	return glm::vec2((v1.x + v2.x) / 2, (v1.y + v2.y) / 2);
}
glm::vec2 Triangle::TangentToNormal(glm::vec2 point1, glm::vec2 point2)
{
	glm::vec2 midpoint = Midpoint(point1, point2);
	glm::vec2 tangent = point2 - point1;
	return glm::vec2(0.0f);
}
void FlipY(glm::vec2* vec)
{
	*vec = glm::vec2(vec->x, -vec->y);
}
Triangle::Triangle(glm::vec2 point1, glm::vec2 point2, glm::vec2 point3)
{
	_points[0] = point1;
	_points[1] = point2;
	_points[2] = point3;

	_normals[0] = Rotate90(glm::normalize(point2 - point1));
	_normals[1] = Rotate90(glm::normalize(point3 - point2));
	_normals[2] = Rotate90(glm::normalize(point1 - point3));

	glm::vec2 sum = point1 + point2 + point3;
	_centre = glm::vec2(sum.x / 3, sum.y / 3);
}
bool Triangle::TestPosition(glm::vec2 pos) // works !!
{
	glm::vec2 min_translation_vector = glm::vec2(10.0f);
	float length = 1000.0f;
	glm::vec2 closest_axis;
	for (glm::vec2 axis : _normals)
	{
		float pmin = glm::dot(axis, _points[0]);
		float pmax = pmin;

		for (int i = 0; i < 3; i++)
		{
			float dot = glm::dot(axis, _points[i]);
			if (dot > pmax)
				pmax = dot;
			if (dot < pmin)
				pmin = dot;
		}
		float projected_distance = glm::dot(axis, pos);
		if (projected_distance > pmax || projected_distance < pmin)
		{
			return false;
		}

		// 
		float right = pmax - projected_distance;
		float left = projected_distance - pmin;
		float min_escape = std::max(abs(left), abs(right));
		if (abs(left) > abs(right) && abs(right) < length)
		{

			length = abs(right);
			min_translation_vector = axis * right;
			closest_axis = axis;

		}
		else if (abs(right) > abs(left) && abs(left) < length)
		{
			length = abs(left);
			min_translation_vector = (axis * -left);
			closest_axis = axis;
		}
	}
	return true;
}
Collision Triangle::BallCollidesWithTriangle(glm::vec2 ball_position, float radius)
{
	glm::vec2 min_translation_vector = glm::vec2(10.0f);
	float length = 1000.0f;
	glm::vec2 closest_axis;
	for (glm::vec2 axis : _normals)
	{
		float pmin = glm::dot(axis, _points[0]);
		float pmax = pmin;

		for (int i = 1; i < 3; i++)
		{
			float dot = glm::dot(axis, _points[i]);
			if (dot > pmax)
				pmax = dot;
			if (dot < pmin)
				pmin = dot;
		}
		float dot = glm::dot(axis, ball_position);
		float bmax = dot + radius;
		float bmin = dot - radius;
		//float projected_distance = glm::dot(axis, pos);
		if (bmax < pmin || bmin > pmax)
			return Collision(glm::vec2(0.0f));

		// ----

		float right = pmax - bmin;
		float left = bmax - pmin;
		float min_escape = std::max(abs(left), abs(right));
		if (abs(left) > abs(right) && abs(right) < length)
		{

			length = abs(right);
			min_translation_vector = axis * right;
			closest_axis = axis; // slightly redundant

		}
		else if (abs(right) > abs(left) && abs(left) < length)
		{
			length = abs(left);
			min_translation_vector = (axis * -left);
			closest_axis = axis;
		}
	}
	// determine closest point, for normal data

	return Collision(min_translation_vector, closest_axis);
}