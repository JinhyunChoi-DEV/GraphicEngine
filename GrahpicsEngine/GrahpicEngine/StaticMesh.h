/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: StaticMesh.h
Purpose: Making of header file for StaticMesh which aim for solid color Model
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <glm/vec3.hpp>

#include "Mesh.h"

// Only have Emissive Color Mesh
class StaticMesh : public Mesh
{
public:
	StaticMesh(std::vector<glm::vec3> vertex_, std::vector<unsigned int> indices_);
	StaticMesh(const StaticMesh& copy);
	~StaticMesh();

	void CopyData(const Mesh& copy) override;
	void Draw(Transform* transform) override;
	void DrawDebug(Transform* transform) override {}

private:
	void ClearBuffer();
	void CreateBuffer();

	std::vector<unsigned int> indices;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};
