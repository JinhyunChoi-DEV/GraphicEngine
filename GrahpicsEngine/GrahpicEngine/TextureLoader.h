/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: TextureLoader.h
Purpose: Making of header file for TextureLoader system
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

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