#ifndef POLYGON_H
#define POLYGON_H

#include "Ball.h" // gives us VAO and EBO
#include"Triangle.h"
#include<vector>


class Polygon : public GameObject
{
	public:
		Polygon(GLfloat* vertices, GLsizeiptr vertex_count, GLuint* triangles, GLsizeiptr triangle_count, Texture2D sprite, glm::vec2 pos = glm::vec2(200.0f, 300.0f), glm::vec2 size = glm::vec2(1.0f), glm::vec3 color = glm::vec3(1.0f));
		std::vector<Triangle> Triangles;
		void ConfigureTriangles(GLfloat* vertices, GLuint* triangles, GLsizeiptr triangle_count);
		//void DrawPolygon();
		void Draw(SpriteRenderer& renderer) override;
		VAO* vao;
		EBO* ebo;
};

#endif