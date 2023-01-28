#include <glad/glad.h>

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Texture::Load(std::string path)
{
	Buffer = stbi_load(path.c_str(), &Width, &Height, &Channel, 0);

	glGenTextures(1, &BufferObject);
	glBindTexture(GL_TEXTURE_2D, BufferObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GLint type = 0;
	if (Channel == 3)
		type = GL_RGB;
	else if (Channel == 4)
		type = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, type, Width, Height, 0, type, GL_UNSIGNED_BYTE, Buffer);
}
