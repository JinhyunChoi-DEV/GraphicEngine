#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "IBoundingVolume.h"

class AABB_BV : public IBoundingVolume
{
public:
	AABB_BV();

	void CreateByMesh(Mesh* mesh) override;
	void Expand(BoundingVolume other) override;
	void Expand(std::vector<BoundingVolume> others) override;
	void Draw() override;
	void Clear() override;

private:
	void CreateBuffer() override;
	void ClearBuffer() override;

	void CreateAABBLines();
	std::vector<glm::vec3> AABBLines;

};
