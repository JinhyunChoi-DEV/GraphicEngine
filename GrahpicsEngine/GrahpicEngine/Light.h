/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Light.h
Purpose: Making of header file for making Light components
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <glm/glm.hpp>
#include "IComponent.h"

enum class LightType
{
	Directional = 0, Point, Spotlight, Count
};

class Object;
class Transform;

class Light : public IComponent
{
public:
	Light(Object*);
	~Light();

	void Initialize() override;
	void Update() override;
	void Delete() override;

	Transform* transform = nullptr;
	LightType Type = LightType::Directional;
	glm::vec3 Direction = glm::vec3(0);
	glm::vec3 Ambient = glm::vec3(0);
	glm::vec3 Diffuse = glm::vec3(0);
	glm::vec3 Specular = glm::vec3(0);
	float InnerAngle = 0.0f;
	float OuterAngle = 0.0f;
	float Fallout = 0.0f;

private:
	Object* root;
};
