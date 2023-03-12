#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "BoundingVolumeHelper.h"
#include "IBoundingVolume.h"

// base on EPOS-14
class LarssonSphere_BV : public IBoundingVolume
{
public:
	LarssonSphere_BV();

	void CreateByMesh(Mesh* mesh) override;
	void Expand(glm::vec3 min_, glm::vec3 max_) override;
	void Expand(BoundingVolume other) override;
	void Expand(std::vector<BoundingVolume> others) override;
	void Draw() override;
	void Clear() override;

private:
	void CreateSphere();
	void CreateBuffer() override;
	void ClearBuffer() override;

	ExtremeData extreme{};
	std::vector<glm::vec3> extremePoints;
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> lineIndices;

	unsigned int EBO = 0;
};