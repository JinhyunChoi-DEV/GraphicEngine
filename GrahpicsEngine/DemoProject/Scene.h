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

	glm::vec3 lightOriginPosition;
	std::vector<std::string> modelNames;
	std::vector<Object*> lights;
	std::string selectedModelName;
	int selectedLightIndex;
	Object* mainObject;
	float rotationTime;
	float lastUpdateTime;
	int activeLightCount;
	bool needResetRotationTimer;
	bool activeTestUV;
	bool activeRotation;

	std::string lightTypeName[3] =
	{
		"Directional",
		"Point",
		"Spotlight"
	};
};
