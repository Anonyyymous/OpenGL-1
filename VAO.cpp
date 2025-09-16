#include "VAO.h"
using namespace std;
#include<iostream>
//
VAO::VAO(GLfloat* vertices, GLsizeiptr size)
{
    glGenVertexArrays(1, &Id);
    Bind();

    Size = size;
    vbo = new VBO(vertices, size);
    cout << "VBO Created" << endl;

    // GLsizei stride = 4 * sizeof(GLfloat);
    //vbo->Bind();

}
void VAO::Configure()
{
    GLsizei stride = 4 * sizeof(float);

    vbo->Bind();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    vbo->Unbind();

    /*glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    //cout << "about to link attrib #1" << endl;
    //LinkAttribute(0, 2, stride, 0);
   //cout << "about to link attrib #2" << endl;
    //LinkAttribute(1, 2, stride, stride / 2);
    //vbo->Unbind();
    //cout << "about to bind vertex arr" << endl;

    glBindVertexArray(Id);
    glEnableVertexAttribArray(0);
    cout << "vertex attrib array 0 enabled" << endl;
    // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);*/

    cout << "buffers cleared" << endl;

}
void VAO::LinkAttribute(GLuint layout, GLint numComponents, GLsizei stride, int offset)
{
    glVertexAttribPointer(layout, numComponents, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(0);
    cout << "attribute linked" << endl;
}
VAO::VAO()
{
    Id = 0;
    Size = 0;
    std::cout << "creating empty VAO" << std::endl;
}

void VAO::Bind()
{
	//glBindBuffer(GL_ARRAY_BUFFER, Id);
    glBindVertexArray(Id);
}
void VAO::Unbind()
{
    glBindVertexArray(Id);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VAO::Delete()
{
    glDeleteVertexArrays(1, &Id);
}