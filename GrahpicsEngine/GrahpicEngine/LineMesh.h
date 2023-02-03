/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: LineMesh.h
Purpose: Making of header file for LineMesh
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <glm/glm.hpp>

#include "Mesh.h"

class LineMesh : public Mesh
{
public:
	LineMesh(std::vector<glm::vec3> vertex_);
	~LineMesh();

	void CopyData(const Mesh& copy) override {}
	void Draw(Transform* transform) override;
	void DrawDebug(Transform* transform) override {}

private:
	void CreateBuffer();
	void ClearBuffer();

	unsigned int VAO;
	unsigned int VBO;
};
