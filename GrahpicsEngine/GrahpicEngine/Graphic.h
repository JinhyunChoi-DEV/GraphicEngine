#pragma once
#include <vector>
#include <glm/glm.hpp>

class Camera;

class Graphic
{
public:
	Graphic();

	void Initialize();
	void Update();
	void Close();

	void AddCamera(Camera*);
	void DeleteCamera(Camera*);

	glm::vec2 ScreenSize;
private:
	void InitializeUniformBuffer();
	void UpdateUniformBuffer(Camera* cam) const;

	Camera* mainCamera;
	std::vector<Camera*> cameras;
	unsigned int uboMatrix;
};

extern Graphic* GRAPHIC;