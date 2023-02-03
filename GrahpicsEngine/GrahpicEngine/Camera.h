/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.h
Purpose: Making of header file for making camera
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <glm/glm.hpp>
#include "IComponent.h"

class Object;
class Transform;

enum class CameraMoveAxis
{
	Front, Back, Right, Left
};

class Camera : public IComponent
{
public:
	Camera(Object* obj);
	~Camera();

	void Initialize() override;
	void Update() override;
	void Delete() override;

	void Move(float speed, CameraMoveAxis axis);
	const glm::mat4 View();
	const glm::mat4 Projection();
	Object* gameObject;
	glm::vec3 Position;

	bool IsMain;
	bool UseConstantRatio;
	float Near, Far, Fov;
private:
	glm::vec3 back, up, right;
	glm::vec3 baseBack, baseRight;
	float ratio, width, height;
};