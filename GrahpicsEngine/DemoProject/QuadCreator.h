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
	auto result = new ModelMesh(vertex, normal, uv, indices);
	result->UseDeferredRendering = true;
	return result;
}