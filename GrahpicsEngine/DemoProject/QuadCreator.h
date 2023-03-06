/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: QuadCreator.h
Purpose: Making of quad mesh
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include "ModelMesh.h"

inline ModelMesh* CreateQuad()
{
	std::vector<glm::vec3> vertex =
	{
		{-1,-1,0 },
		{1,-1,0 },
		{1,1,0 },
		{-1,-1,0 },
		{1,1,0 },
		{-1,1,0 },
	};

	std::vector<glm::vec3> normal =
	{
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1}
	};

	std::vector<glm::vec2> uv =
	{
		{0,0},
		{1,0},
		{1,1},
		{0,0},
		{1,1},
		{0,1}
	};

	std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5 };
	auto result = new ModelMesh(vertex, normal, uv, indices, false);
	result->UseDeferredRendering = true;
	return result;
}