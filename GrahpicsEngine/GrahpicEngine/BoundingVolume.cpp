#include "BoundingVolume.h"
#include "Object.h"

BoundingVolume::BoundingVolume()
{
	ActiveAABB = true;
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

void BoundingVolume::Expand(glm::vec3 min, glm::vec3 max)
{
	aabb.Expand(min, max);
	ritterShpere.Expand(min, max);
	larssonSphere.Expand(min, max);
	pcaSphere.Expand(min, max);
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

IBoundingVolume* BoundingVolume::GetBV(BoundingVolumeType type)
{
	if (type == BoundingVolumeType::AABB)
		return &aabb;

	if (type == BoundingVolumeType::Ritter_BS)
		return &ritterShpere;

	if (type == BoundingVolumeType::Larsson_BS)
		return &larssonSphere;

	if (type == BoundingVolumeType::PCA_BS)
		return &pcaSphere;

	std::cout << "ERROR: INCOMPLETE TYPE OF BV " << std::endl;
	return new AABB_BV();
}

void BoundingVolume::SetColor(glm::vec3 color)
{
	aabb.lineColor = color;
	ritterShpere.lineColor = color;
	larssonSphere.lineColor = color;
	pcaSphere.lineColor = color;
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
	//if (ActiveAABB)
		//aabb.Draw();

	//if (ActiveRitterSphere)
		//ritterShpere.Draw();

	//if (ActiveLarssonSphere)
	//larssonSphere.Draw();

	//if (ActivePCAShpere)
	pcaSphere.Draw();
}
