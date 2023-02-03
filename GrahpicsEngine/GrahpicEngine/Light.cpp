/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Light.cpp
Purpose: Making of implementing for Lighting components.
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#include "Light.h"
#include "Graphic.h"
#include "Object.h"


Light::Light(Object* root_)
{
	root = root_;

	Direction = glm::vec3(0);
	Ambient = glm::vec3(1);
	Diffuse = glm::vec3(1);
	Specular = glm::vec3(1);
	Type = LightType::Directional;
	InnerAngle = 0.0f;
	OuterAngle = 0.0f;
	Fallout = 1.0f;
	transform = root_->transform;
}

Light::~Light()
{
	GRAPHIC->DeleteLight(this);
}

void Light::Initialize()
{
	GRAPHIC->AddLight(root, this);
}

void Light::Update()
{
}

void Light::Delete()
{
}
