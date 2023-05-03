#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include "stb_image.h"

#include "Shader.h"

class Texture
{
public:
	unsigned int ID;
	GLenum textureType;

	Texture(const char* texturePath, GLenum textureType, GLenum textureSlot);

	void Bind();

	void UnBind();

	void Delete();

};

#endif // !TEXTURE_CLASS_H

