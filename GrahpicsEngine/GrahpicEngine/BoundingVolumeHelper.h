#pragma once
#include <tuple>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include "Mesh.h"

struct ExtremeData
{
	glm::vec3 center = glm::vec3(0);
	float radius = 0;
};

inline std::tuple<glm::vec3, glm::vec3> ExtremePoint(Mesh* mesh, glm::vec3 dir)
{
	auto vertex = mesh->Vertex();
	glm::vec3 min, max;
	float projMin = FLT_MAX, projMax = -FLT_MAX;

	for (auto v : vertex)
	{
		float d = glm::dot(dir, v);

		if (d < projMin)
		{
			projMin = d;
			min = v;
		}

		if (d >= projMax)
		{
			projMax = d;
			max = v;
		}
	}

	return { min, max };
}

inline void GrowSphere(ExtremeData& extreme, Mesh* mesh)
{
	auto vertex = mesh->Vertex();
	for (auto v : vertex)
	{
		auto center = extreme.center;
		float distance = glm::distance(center, v);

		if (distance > extreme.radius)
		{
			glm::vec3 d = glm::normalize(v - center);
			float d_distance = glm::length(v - center);
			float newRadius = 0.5f * (extreme.radius + d_distance);
			glm::vec3 newCenter = center + (newRadius - extreme.radius) * d;

			extreme.center = newCenter;
			extreme.radius = newRadius;
		}
	}
}

inline std::tuple<std::vector<glm::vec3>, std::vector<unsigned int>> CreateBoundingSphere(ExtremeData extreme, int stack, int sector)
{
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> lineIndices;

	constexpr auto pi = glm::pi<float>();
	float sectorStep = 2 * pi / (float)sector;
	float stackStep = pi / (float)stack;
	float sectorAngle, stackAngle = 0.0f;

	for (int i = 0; i <= stack; ++i)
	{
		stackAngle = pi / 2 - (float)i * stackStep;
		float xz = extreme.radius * cosf(stackAngle);
		float y = extreme.radius * sinf(stackAngle) + extreme.center.y;

		for (int j = 0; j <= sector; ++j)
		{
			sectorAngle = (float)j * sectorStep;
			float z = xz * cosf(sectorAngle) + extreme.center.z;
			float x = xz * sinf(sectorAngle) + extreme.center.x;
			glm::vec3 currentVertex(x, y, z);
			vertices.push_back(currentVertex);
		}
	}

	int k1, k2;
	for (int i = 0; i < stack; ++i)
	{
		k1 = i * (sector + 1);
		k2 = k1 + sector + 1;

		for (int j = 0; j < sector; ++j, ++k1, ++k2)
		{
			lineIndices.push_back(k1);
			lineIndices.push_back(k2);

			if (i != 0)
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
		}
	}

	return { vertices, lineIndices };
}
