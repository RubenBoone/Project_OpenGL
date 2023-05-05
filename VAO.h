#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	unsigned int ID;

	VAO();

	void AddAttrib(VBO& vbo, unsigned int layoutPosition, unsigned int size, GLenum dataType, GLsizei stride, void* offset);
	void LinkVBO();
	void Bind();
	void Unbind();
	void CleanUp();
};

#endif // !VAO_CLASS_H
