/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Assignment1_Stage.h
Purpose: Making of header file for making Assignment1 scene
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include "Object.h"
#include <glm/glm.hpp>

class Assignment1_Stage
{
public:
	void Initialize();
	void Update();
	void Close();

private:
	void InitializeGUI();
	void LoadAllModel();
	void CreateCamera();
	void CreateDeferredObject();
	void CreateOrbit();
	void CreateLight();

	void RotateLights(float dt);
	void UpdateCamera(float dt);
	void UpdateLights();
	void UpdateGUI();
	void UpdateModelGUI();
	void UpdateLightGUI();
	void UpdateDeferredRenderGUI();
	void UpdateDebugViewGUI();

	void SetScenario1();
	void SetScenario2();
	void SetScenario3();

	glm::vec3 lightOriginPosition = glm::vec3(0);
	std::vector<std::string> modelNames;
	std::vector<Object*> lights;
	std::string selectedModelName;
	int selectedLightIndex = 0;
	Object* mainObject = nullptr;
	float rotationTime = 0.0f;
	float lastUpdateTime = 0.0f;
	int activeLightCount = 0;
	bool needResetRotationTimer = false;
	bool activeTestUV = false;
	bool activeRotation = false;

	std::string lightTypeName[3] =
	{
		"Directional",
		"Point",
		"Spotlight"
	};
};
