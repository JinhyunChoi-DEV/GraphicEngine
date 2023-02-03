/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ShaderManager.h
Purpose: Making of header file for ShaderManager System to easily compile and get shader data
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <memory>
#include <unordered_map>

class Shader;

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void Compile(std::string name, std::string vertex, std::string fragment);
	void Recompile(std::string name);
	void Use(std::string name);

	Shader* Get(std::string name);

private:
	bool IsExist(std::string name);
	std::unordered_map < std::string, std::unique_ptr<Shader>> shaders;
};

extern ShaderManager* SHADERS;
