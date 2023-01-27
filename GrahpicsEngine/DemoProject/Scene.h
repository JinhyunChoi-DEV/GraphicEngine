#pragma once
#include "Object.h"
#include <glm/glm.hpp>

class Scene
{
public:
	void Initialize();
	void Update();
	void Close();

private:
	void LoadAllModel();
	void CreateOrbit();
	void CreateLight();
	void UpdateLights(float dt);

	glm::vec3 lightOriginPosition;
	std::vector<std::string> modelNames;
	std::vector<Object*> lights;
	Object* mainObject;
	float rotationTime;
	bool needResetRotationTimer;
	float lastUpdateTime;
	int activeLightCount;
};
