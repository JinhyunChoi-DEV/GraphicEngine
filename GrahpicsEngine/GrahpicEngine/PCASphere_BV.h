/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: PCASphere_BV.h
Purpose: Making of header file for making PCA method sphere
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
#include "BoundingVolumeHelper.h"

class Mesh;
class PCASphere_BV : public IBoundingVolume
{
public:
	PCASphere_BV();

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

	void CreateSphere();
	void CreateCovarianceMat(Mesh* mesh);
	bool CheckConvergence(glm::mat3 mat, float& prevSum);
	void FindMaximumElement(glm::mat3 mat);
	glm::mat3 GetJacobiMatrix(float c, float s);
	glm::vec3 GetEigenVector(glm::mat3 V);

	int p = 0, q = 0;
	glm::mat3 covarianceMatrix = glm::mat3(1.0f);
	ExtremeData extreme{};

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> lineIndices;
	unsigned int EBO = 0;
};