#pragma once
#include <glm/glm.hpp>

class Mesh;
class BoundingVolume;

class IBoundingVolume
{
public:
	virtual void CreateByMesh(Mesh* mesh) = 0;
	virtual void Expand(glm::vec3 min_, glm::vec3 max_) = 0;
	virtual void Expand(BoundingVolume other) = 0;
	virtual void Expand(std::vector<BoundingVolume> others) = 0;
	virtual float Volume() = 0;
	virtual void Draw() = 0;
	virtual void Clear() = 0;

	glm::vec3 min{ FLT_MAX };
	glm::vec3 max{ -FLT_MAX };
	glm::vec3 center{ 0 };
	glm::vec3 lineColor{ 0 };
protected:
	virtual void CreateBuffer() = 0;
	virtual void ClearBuffer() = 0;

	unsigned int VAO = 0;
	unsigned int VBO = 0;
};