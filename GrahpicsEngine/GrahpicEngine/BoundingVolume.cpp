/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingVolume.cpp
Purpose: Making of source file for implementing master bounding volume controller
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

#include "BoundingVolume.h"
#include "Object.h"

std::vector<glm::vec3> BoundingVolume::BottomUpColors;
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

void BoundingVolume::ActiveByType(BoundingVolumeType type)
{
	ActiveAABB = type == BoundingVolumeType::AABB;
	ActiveRitterSphere = type == BoundingVolumeType::Ritter_BS;
	ActiveLarssonSphere = type == BoundingVolumeType::Larsson_BS;
	ActivePCAShpere = type == BoundingVolumeType::PCA_BS;
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
	if (ActiveAABB)
		aabb.Draw();

	if (ActiveRitterSphere)
		ritterShpere.Draw();

	if (ActiveLarssonSphere)
		larssonSphere.Draw();

	if (ActivePCAShpere)
		pcaSphere.Draw();
}
