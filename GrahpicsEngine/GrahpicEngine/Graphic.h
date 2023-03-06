/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Graphic.h
Purpose: Making of header file for Graphic system
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "BoundingVolumeHierarchy.h"

class Camera;
class Light;
class Mesh;
class Object;

struct LightGlobalData
{
	glm::vec3 attenuationConstant = glm::vec3(0);
	glm::vec3 globalAmbient = glm::vec3(0);
	glm::vec3 fog = glm::vec3(0);
	float minFog = 0.0f;
	float maxFog = 0.0f;
};

class Graphic
{
public:
	Graphic();

	void Initialize();
	void Update();
	void Close();

	void AddLight(Object* root, Light* light);
	void DeleteLight(Light* light);

	void AddCamera(Camera*);
	void DeleteCamera(Camera*);
	void DrawDeferredView();
	void SetCurrentBVH(BoundingVolumeHierarchy* bvh);
	Camera* MainCamera();

	LightGlobalData LightData;
	glm::vec2 ScreenSize = glm::vec2(0);

	bool ActiveCopyDepthBuffer = false;
	bool ActiveDrawFSQ = false;

private:
	void RenderDeferred(std::vector<Object*>);
	void RenderForward(std::vector<Object*>, std::vector<Object*>);

	void InitializeDeferredRender();
	void InitializeUniformBuffer();
	void UpdateTransformUniformBuffer(Camera* cam) const;
	void UpdateLightingUniformBuffer();

	BoundingVolumeHierarchy* currentBVH = nullptr;
	Camera* mainCamera = nullptr;
	Mesh* fsqMesh = nullptr;
	std::vector<Camera*> cameras;
	std::vector<std::pair<Object*, Light*>> lights;
	unsigned int uboTransform = 0;
	unsigned int uboLighting = 0;
	unsigned int gBufferFBO = 0;
	unsigned int* gBufferTextures = nullptr;
	unsigned int renderBufferDepth = 0;
	const unsigned int NUM_ATTACHMENT = 8;
};

extern Graphic* GRAPHIC;