/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingVolume.h
Purpose: Making of header file for implementing master bounding volume controller
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

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
	void ActiveByType(BoundingVolumeType type);

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

	static std::vector<glm::vec3> BottomUpColors;
private:
};
