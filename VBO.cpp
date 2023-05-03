#include "VBO.h"

VBO::VBO(float* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::CleanUp()
{
	glDeleteBuffers(1, &ID);
}