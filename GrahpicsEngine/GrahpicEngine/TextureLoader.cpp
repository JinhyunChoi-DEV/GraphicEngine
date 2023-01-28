#include <stb_image.h>

#include "TextureLoader.h"
#include "Texture.h"

TextureLoader* TEXTURES = nullptr;
TextureLoader::TextureLoader()
{
	if (TEXTURES == nullptr)
		TEXTURES = this;

	stbi_set_flip_vertically_on_load(true);
}

TextureLoader::~TextureLoader()
{
	textures.clear();
}

void TextureLoader::Load(std::string name, std::string fileName)
{
	std::string path = basePath + fileName;

	Texture texture;
	texture.Load(path);

	textures.insert(std::make_pair(name, texture));
}

Texture TextureLoader::Get(std::string name)
{
	Texture defaultValue;

	if (textures.count(name) <= 0)
		return defaultValue;

	return textures[name];
}
