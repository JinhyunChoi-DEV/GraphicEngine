/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: TextureLoader.cpp
Purpose: Making of implementing TextureLoader System to easily load the images
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

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
