#pragma once
#include "AABB_BV.h"
#include "BoundingVolumeHierarchyHelper.h"
#include "RitterSphere_BV.h"
#include "LarssonSphere_BV.h"
#include "IComponent.h"
#include "PCASphere_BV.h"

class Object;
class BoundingVolume : public IComponent
{
public:
	BoundingVolume();

	void CreateByMesh(Object* base_);
	void Expand(glm::vec3 min, glm::vec3 max);
	void Expand(BoundingVolume other);
	void Expand(std::vector<BoundingVolume> others);

	IBoundingVolume* GetBV(BoundingVolumeType type);

	void SetColor(glm::vec3 color);

	void Initialize() override {}
	void Update() override {}
	void Delete() override;

	void Draw();

	bool ActiveAABB;
	bool ActiveRitterSphere;
	bool ActiveLarssonSphere;
	bool ActivePCAShpere;

	AABB_BV aabb{};
	RitterSphere_BV ritterShpere{};
	LarssonSphere_BV larssonSphere{};
	PCASphere_BV pcaSphere{};
private:
};
