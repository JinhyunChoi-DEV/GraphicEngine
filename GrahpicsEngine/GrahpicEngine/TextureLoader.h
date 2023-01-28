#pragma once
#include <unordered_map>

class Texture;

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	void Load(std::string name, std::string fileName);

	Texture Get(std::string name);

private:
	std::string basePath = "textures/";
	std::unordered_map<std::string, Texture> textures;
};

extern TextureLoader* TEXTURES;