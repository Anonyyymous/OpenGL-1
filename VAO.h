#ifndef VAO_H
#define VAO_H

#include "EBO.h"
#include "VBO.h"

class VAO
{
public:
	GLuint Id;
	GLuint Size;
	VBO* vbo;
	VAO(GLfloat* vertices, GLsizeiptr size);
	VAO();

	void LinkAttribute(GLuint layout, GLint numComponents, GLsizei stride, int offset);
	void Bind();
	void Unbind();
	void Delete();
	void Configure();
};

#endif