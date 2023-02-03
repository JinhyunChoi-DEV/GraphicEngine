/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.h
Purpose: Making of header file for Texture data
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <string>

class Texture
{
public:
	Texture() = default;

	void Load(std::string path);

	int Width = 0;
	int Height = 0;
	int Channel = 0;
	unsigned int BufferObject = -1;
	unsigned char* Buffer = nullptr;
};
