#pragma once
#include <glm/glm.hpp>

class Mesh;
class BoundingVolume;

class IBoundingVolume
{
public:
	IBoundingVolume()
	{
		min = glm::vec3(FLT_MAX);
		max = glm::vec3(-FLT_MAX);
		center = glm::vec3(0);
	}

	virtual void CreateByMesh(Mesh* mesh) = 0;
	virtual void Expand(glm::vec3 min_, glm::vec3 max_) = 0;
	virtual void Expand(BoundingVolume other) = 0;
	virtual void Expand(std::vector<BoundingVolume> others) = 0;
	virtual void Draw() = 0;
	virtual void Clear() = 0;

	glm::vec3 lineColor;

	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 center;
protected:
	virtual void CreateBuffer() = 0;
	virtual void ClearBuffer() = 0;

	unsigned int VAO = 0;
	unsigned int VBO = 0;
};