#include "Polygon.h"
#include<iostream>


Polygon::Polygon(GLfloat* vertices, GLsizeiptr vertex_count, GLuint* triangles, GLsizeiptr triangle_count, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color) : GameObject(pos, size, sprite, color, glm::vec2(0.0f))
{
	ConfigureTriangles(vertices, triangles, triangle_count / sizeof(GLuint));
	vao = new VAO(vertices, vertex_count);
	std::cout << "VAO created successfully" << std::endl;
	ebo = new EBO(triangles, triangle_count);
	std::cout << "EBO created successfully" << std::endl;
	vao->Configure();
	vao->Unbind();
	vao->vbo->Unbind();
	ebo->Unbind();
	std::cout << "polygon created successfully" << std::endl;

	// ConfigureTriangles(vertices, triangles, triangle_count / sizeof(GLuint));
	//Sprite = ResourceManager.GetTexture("background");

}
void Polygon::ConfigureTriangles(GLfloat* vertices, GLuint* triangles, GLsizeiptr triangle_count)
{
	int stride = 4;
	for (int i = 0; i < triangle_count; i += 3)
	{
		Triangles.push_back(Triangle(
			glm::vec2(vertices[triangles[i] * 4], vertices[(triangles[i] * 4) + 1]),
			glm::vec2(vertices[triangles[i+1] * 4], vertices[(triangles[i+1] * 4) + 1]),
			glm::vec2(vertices[triangles[i+2] * 4], vertices[(triangles[i+2] * 4) + 1])
		));
		std::cout << "triangle configured" << std::endl;
	}
}
void Polygon::Draw(SpriteRenderer& renderer)
{
	//renderer.DrawSprite(this);
	// std::cout << "Trying to draw at rotation " << this->Rotation << std::endl;
	renderer.DrawPolygon(vao, ebo, this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
	// std::cout << "drawing polygon" << std::endl;
	// id = id;
}