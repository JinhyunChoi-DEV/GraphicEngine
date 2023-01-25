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
	float x = std::min(v1.x, v2.x);
	float y = std::min(v1.y, v2.y);
	float z = std::min(v1.z, v2.z);
	return { x, y, z };
}

inline glm::vec3 GetMax(glm::vec3 v1, glm::vec3 v2)
{
	float x = std::max(v1.x, v2.x);
	float y = std::max(v1.y, v2.y);
	float z = std::max(v1.z, v2.z);
	return { x, y, z };
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