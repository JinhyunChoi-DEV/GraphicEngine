/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Material.h
Purpose: Making of header file for Material components
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <glm/glm.hpp>
#include "Texture.h"

class Shader;

class Material
{
public:
	Material();

	void Update(Shader*);

	Texture AmbientTexture;
	Texture DiffuseTexture;
	Texture SpecularTexture;

	glm::vec3 AmbientColor;
	glm::vec3 DiffuseColor;
	glm::vec3 SpecularColor;
	glm::vec3 Intensity;

	bool UseGpuUV;
private:
	Texture baseDefault;

};