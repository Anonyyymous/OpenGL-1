#ifndef EBO_H
#define EBO_H

#include<glad/glad.h>
#include<iostream>

class EBO { // Element Buffer Object
	public:
		EBO(GLuint* indices, GLsizeiptr size);
		EBO();
		GLuint Id;
		GLuint Size;
		void Bind();
		void Unbind();
		void Delete();
};

#endif
