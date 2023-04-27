#ifndef TEXTURE

#include <glad/glad.h>
#include "stb_image.h"

#endif // !TEXTURE

class Texture
{
public:
	unsigned int ID;

	Texture(const char* texturePath);

	void bindTexture();

	void deleteTexture();

};

