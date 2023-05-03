#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
}

void VAO::AddAttrib(VBO& vbo, unsigned int layoutPosition, unsigned int size, GLenum dataType, GLsizei stride, void* offset)
{
	vbo.Bind();

	glVertexAttribPointer(layoutPosition, size, dataType, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layoutPosition);

	vbo.UnBind();

}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::CleanUp()
{
	glDeleteVertexArrays(1, &ID);
}