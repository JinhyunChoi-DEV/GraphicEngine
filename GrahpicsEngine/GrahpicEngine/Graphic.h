#pragma once
#include <vector>
#include <glm/glm.hpp>

class Camera;
class Light;

struct LightGlobalData
{
	glm::vec3 attenuationConstant;
	glm::vec3 globalAmbient;
	glm::vec3 fog;
	float minFog;
	float maxFog;
};

class Graphic
{
public:
	Graphic();

	void Initialize();
	void Update();
	void Close();

	void AddLight(Light* light);
	void DeleteLight(Light* light);

	void AddCamera(Camera*);
	void DeleteCamera(Camera*);
	const Camera* MainCamera();

	LightGlobalData LightData;
	glm::vec2 ScreenSize;
private:
	void InitializeUniformBuffer();
	void UpdateTransformUniformBuffer(Camera* cam) const;
	void UpdateLightingUniformBuffer();

	Camera* mainCamera;
	std::vector<Camera*> cameras;
	std::vector<Light*> lights;
	unsigned int uboTransform;
	unsigned int uboLighting;
};

extern Graphic* GRAPHIC;