/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.cpp
Purpose: Making of implementing for Transform component including position, scale, and rotation
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#include <glm/gtx/quaternion.hpp>

#include "Transform.h"

Transform::Transform()
{
	Position = glm::vec3{ 0,0,0 };
	Scale = glm::vec3{ 1,1,1 };
	Rotation = glm::vec3{ 0,0,0 };
}

const glm::mat4 Transform::GetTransform()
{
	float xAngle = glm::radians(Rotation.x);
	float yAngle = glm::radians(Rotation.y);
	float zAngle = glm::radians(Rotation.z);
	glm::vec3 rotationRadians = glm::vec3(xAngle, yAngle, zAngle);
	auto rotateQuat = glm::quat(rotationRadians);

	glm::mat4 identicalMatrix = glm::mat4(1.0f);
	glm::mat4 positionMatrix = glm::translate(identicalMatrix, Position);
	glm::mat4 scaleMatrix = glm::scale(identicalMatrix, Scale);
	glm::mat4 rotateMatrix = glm::toMat4(rotateQuat);

	return positionMatrix * rotateMatrix * scaleMatrix;
}
