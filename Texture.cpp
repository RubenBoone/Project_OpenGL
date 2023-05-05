#include "Texture.h"

Texture::Texture(const char* texturePath, GLenum textureType, GLenum textureSlot) {

	Texture::textureType = textureType;

	int width, height, nColChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(texturePath, &width, &height, &nColChannels, 0);

	glGenTextures(1, &ID);
	glActiveTexture(textureSlot);
	Texture::Bind();

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(textureType, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(textureType);
	stbi_image_free(bytes);

	Texture::UnBind();
}

void Texture::Bind() {
	glBindTexture(textureType, ID);
}

void Texture::UnBind() {
	glBindTexture(textureType, 0);
}

void Texture::CleanUp() {
	glDeleteTextures(1, &ID);
}