/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: IBoundingVolume.h
Purpose: Making of header file for making interface class for bounding volume
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

#pragma once
#include <glm/glm.hpp>

class Mesh;
class BoundingVolume;

class IBoundingVolume
{
public:
	virtual void CreateByMesh(Mesh* mesh) = 0;
	virtual void Expand(glm::vec3 min_, glm::vec3 max_) = 0;
	virtual void Expand(BoundingVolume other) = 0;
	virtual void Expand(std::vector<BoundingVolume> others) = 0;
	virtual float Volume() = 0;
	virtual void Draw() = 0;
	virtual void Clear() = 0;

	glm::vec3 min{ FLT_MAX };
	glm::vec3 max{ -FLT_MAX };
	glm::vec3 center{ 0 };
	glm::vec3 lineColor{ 0 };
protected:
	virtual void CreateBuffer() = 0;
	virtual void ClearBuffer() = 0;

	unsigned int VAO = 0;
	unsigned int VBO = 0;
};