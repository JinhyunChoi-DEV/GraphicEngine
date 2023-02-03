/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.h
Purpose: Making of header file for Transform component
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once

#include <glm/glm.hpp>

class Transform
{
public:
	Transform();

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 Rotation;

	const glm::mat4 GetTransform();
private:
};