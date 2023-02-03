/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ModelTransformFixer.h
Purpose: Making of header file for fixing models vertex area(-1 to 1) and set to origin(0,0) position
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <vector>
#include <glm/vec3.hpp>

struct FixerData
{
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 sumAllVertex;
};

inline glm::vec3 GetMin(glm::vec3 v1, glm::vec3 v2)
{
	if (glm::length(v1) > glm::length(v2))
		return v2;

	return v1;
}

inline glm::vec3 GetMax(glm::vec3 v1, glm::vec3 v2)
{
	if (glm::length(v1) > glm::length(v2))
		return v1;

	return v2;
}

inline std::vector<glm::vec3> SetToUnitVertex(FixerData data, std::vector<glm::vec3> vertex)
{
	std::vector<glm::vec3> result;

	int size = static_cast<int>(vertex.size());
	glm::vec3 min = data.min;
	glm::vec3 max = data.max;
	glm::vec3 sumAllVertex = data.sumAllVertex;

	sumAllVertex /= size;
	glm::vec3 originFactor = glm::vec3(0) - sumAllVertex;
	min += originFactor;
	max += originFactor;

	float baseFactor = std::max({ abs(min.x), abs(max.x), abs(min.y), abs(max.y), abs(min.z), abs(max.z) });
	float factor = 1.0f / baseFactor;

	for (auto& point : vertex)
	{
		point += originFactor;
		float x = point.x * factor;
		float y = point.y * factor;
		float z = point.z * factor;

		result.push_back({ x, y, z });
	}

	return result;
}