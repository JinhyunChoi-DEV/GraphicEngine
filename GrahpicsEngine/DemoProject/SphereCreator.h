#pragma once
#include <glm/ext/scalar_constants.hpp>

#include "ModelTransformFixer.h"
#include "StaticMesh.h"

inline StaticMesh* CreateSphere(float radius, int stack, int sector)
{
	constexpr auto pi = glm::pi<float>();
	float sectorStep = 2 * pi / (float)sector;
	float stackStep = pi / (float)stack;
	float sectorAngle, stackAngle = 0.0f;
	float minValue = std::numeric_limits<float>::max();
	float maxValue = std::numeric_limits<float>::min();

	std::vector<glm::vec3> positions;
	std::vector<unsigned int> indices;
	FixerData data;
	data.min = glm::vec3(minValue, minValue, minValue);
	data.max = glm::vec3(maxValue, maxValue, maxValue);
	data.sumAllVertex = glm::vec3(0, 0, 0);

	for (int i = 0; i <= stack; ++i)
	{
		stackAngle = pi / 2 - (float)i * stackStep;
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);

		for (int j = 0; j <= sector; ++j)
		{
			sectorAngle = (float)j * sectorStep;
			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);
			glm::vec3 currentVertex(x, y, z);

			data.sumAllVertex.x += currentVertex.x;
			data.sumAllVertex.y += currentVertex.y;
			data.sumAllVertex.z += currentVertex.z;

			data.min = GetMin(currentVertex, data.min);
			data.max = GetMax(currentVertex, data.max);

			positions.push_back(currentVertex);
		}
	}

	// Index
	for (int i = 0; i < stack; ++i)
	{
		int k1 = i * (sector + 1);
		int k2 = k1 + sector + 1;

		for (int j = 0; j < sector; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stack - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	SetToUnitVertex(data, positions);
	return new StaticMesh(positions, indices);
}
