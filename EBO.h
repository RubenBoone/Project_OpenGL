#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO
{
public:
	unsigned int ID;

	EBO(unsigned int* indices, GLsizeiptr size);

	void Bind();
	void UnBind();
	void CleanUp();
};

#endif // !EBO_CLASS_H