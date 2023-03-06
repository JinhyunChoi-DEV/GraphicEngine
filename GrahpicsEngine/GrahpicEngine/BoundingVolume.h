#pragma once
#include "AABB_BV.h"
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
	void Expand(BoundingVolume other);
	void Expand(std::vector<BoundingVolume> others);

	void Initialize() override {}
	void Update() override {}
	void Delete() override;

	void Draw();

	bool ActiveAABB;
	bool ActiveRitterSphere;
	bool ActiveLarssonSphere;
	bool ActivePCAShpere;

private:
	AABB_BV aabb{};
	RitterSphere_BV ritterShpere{};
	LarssonSphere_BV larssonSphere{};
	PCASphere_BV pcaSphere{};
};
