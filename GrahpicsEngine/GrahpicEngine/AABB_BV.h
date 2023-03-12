/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: AABB_BV.h
Purpose: Making of header file for making bounding volume of AABB
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "IBoundingVolume.h"

class AABB_BV : public IBoundingVolume
{
public:
	AABB_BV();

	void CreateByMesh(Mesh* mesh) override;
	void Expand(glm::vec3 min_, glm::vec3 max_) override;
	void Expand(BoundingVolume other) override;
	void Expand(std::vector<BoundingVolume> others) override;
	float Volume() override;
	void Draw() override;
	void Clear() override;

private:
	void CreateBuffer() override;
	void ClearBuffer() override;

	void CreateAABBLines();
	std::vector<glm::vec3> AABBLines;

};
