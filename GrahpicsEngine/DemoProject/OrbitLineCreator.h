#pragma once
#include <glm/ext/scalar_constants.hpp>

#include "ModelTransformFixer.h"
#include "LineMesh.h"

inline LineMesh* CreateOrbitLine(float radius, int sector)
{
	float minValue = std::numeric_limits<float>::max();
	float maxValue = std::numeric_limits<float>::min();

	std::vector<glm::vec3> positions;
	FixerData data;
	data.min = glm::vec3(minValue, minValue, minValue);
	data.max = glm::vec3(maxValue, maxValue, maxValue);
	data.sumAllVertex = glm::vec3(0, 0, 0);

	glm::vec3 currentVertex(0, 0, 0);
	constexpr float pi = glm::pi<float>();
	float step = 2 * pi / sector;
	for (int i = 0; i <= sector; ++i)
	{
		if (i > 1)
			positions.push_back(currentVertex);

		float angle = step * i;
		currentVertex.x = radius * cosf(angle);
		currentVertex.y = 0.0f;
		currentVertex.z = radius * sinf(angle);

		data.sumAllVertex.x += currentVertex.x;
		data.sumAllVertex.y += currentVertex.y;
		data.sumAllVertex.z += currentVertex.z;

		data.min = GetMin(currentVertex, data.min);
		data.max = GetMax(currentVertex, data.max);

		positions.push_back(currentVertex);
	}

	SetToUnitVertex(data, positions);
	return new LineMesh(positions);
}