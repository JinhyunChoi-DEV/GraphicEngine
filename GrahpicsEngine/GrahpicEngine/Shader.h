/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.h
Purpose: Making of header file for Shader data
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(std::string name, std::string vertex, std::string fragment);
	~Shader();

	void Recompile();
	void Use();
	void Set(const std::string& name, int value) const;
	void Set(const std::string& name, unsigned int value) const;
	void Set(const std::string& name, bool value) const;
	void Set(const std::string& name, float value) const;
	void Set(const std::string& name, glm::vec3 value) const;
	void Set(const std::string& name, glm::vec4 value) const;
	void Set(const std::string& name, glm::mat4 value) const;

	unsigned int ProgramID = 0;
	std::string Name;

private:
	void Compile();

	const std::string filePath = "shaders/";
	const std::string versionText = "#version 430 core\n";
	std::string vertex = "";
	std::string fragment = "";
};
