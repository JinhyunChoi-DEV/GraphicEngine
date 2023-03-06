/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: Making of header file for Mesh interface
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Material.h"

class Transform;
class Shader;

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	virtual void CopyData(const Mesh& copy) = 0;
	virtual void Draw(Transform* transform) = 0;
	virtual void DrawDebug(Transform* transform) = 0;

	bool UseDeferredRendering;
	Shader* shader = nullptr;
	Material* material;
	std::string Name;
	const std::vector<glm::vec3> Vertex() { return vertex; }

protected:
	std::vector<glm::vec3> vertex;
};

inline Mesh::Mesh()
{
	UseDeferredRendering = false;
	material = new Material();
}

inline Mesh::~Mesh()
{
	vertex.clear();
}
