/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: RitterSphere.h
Purpose: Making of header file for making Ritter's method sphere
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "BoundingVolumeHelper.h"
#include "IBoundingVolume.h"

class RitterSphere_BV : public IBoundingVolume
{
public:
	RitterSphere_BV();

	void CreateByMesh(Mesh* mesh) override;
	void Expand(glm::vec3 min_, glm::vec3 max_) override;
	void Expand(BoundingVolume other) override;
	void Expand(std::vector<BoundingVolume> others) override;
	float Volume() override;
	void Draw() override;
	void Clear() override;

	ExtremeData extreme{};
private:
	void CreateSphere();

	void CreateBuffer() override;
	void ClearBuffer() override;

	//	ExtremeData extreme{};
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> lineIndices;
	unsigned int EBO = 0;
};