#include "BoundingVolume.h"
#include "Object.h"

BoundingVolume::BoundingVolume()
{
	ActiveAABB = false;
	ActiveRitterSphere = false;
	ActiveLarssonSphere = false;
	ActivePCAShpere = false;
}

void BoundingVolume::CreateByMesh(Object* base)
{
	aabb.CreateByMesh(base->mesh);
	ritterShpere.CreateByMesh(base->mesh);
	larssonSphere.CreateByMesh(base->mesh);
	pcaSphere.CreateByMesh(base->mesh);
}

void BoundingVolume::Expand(BoundingVolume other)
{
	aabb.Expand(other);
	ritterShpere.Expand(other);
	larssonSphere.Expand(other);
	pcaSphere.Expand(other);
}

void BoundingVolume::Expand(std::vector<BoundingVolume> others)
{
	aabb.Expand(others);
	ritterShpere.Expand(others);
	larssonSphere.Expand(others);
	pcaSphere.Expand(others);
}

void BoundingVolume::Delete()
{
	aabb.Clear();
	ritterShpere.Clear();
	larssonSphere.Clear();
	pcaSphere.Clear();
}

void BoundingVolume::Draw()
{
	if (ActiveAABB)
		aabb.Draw();

	if (ActiveRitterSphere)
		ritterShpere.Draw();

	if (ActiveLarssonSphere)
		larssonSphere.Draw();

	if (ActivePCAShpere)
		pcaSphere.Draw();
}
