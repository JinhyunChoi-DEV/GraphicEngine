#include <glad/glad.h>

#include "AABB_BV.h"
#include "Object.h"
#include "Mesh.h"
#include "ModelTransformFixer.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "BoundingVolume.h"

AABB_BV::AABB_BV()
{
	lineColor = glm::vec3(1, 0, 0);
}

void AABB_BV::CreateByMesh(Mesh* mesh)
{
	auto vertex = mesh->Vertex();
	for (auto v : vertex)
	{
		min.x = std::min(min.x, v.x);
		min.y = std::min(min.y, v.y);
		min.z = std::min(min.z, v.z);

		max.x = std::max(max.x, v.x);
		max.y = std::max(max.y, v.y);
		max.z = std::max(max.z, v.z);
	}
	center = (min + max) / 2.0f;

	CreateAABBLines();
	CreateBuffer();
}

void AABB_BV::Expand(glm::vec3 min_, glm::vec3 max_)
{
	min = glm::min(min, min_);
	max = glm::max(max, max_);
	center = (min + max) / 2.0f;

	AABBLines.clear();
	ClearBuffer();
	CreateAABBLines();
	CreateBuffer();
}

void AABB_BV::Expand(BoundingVolume other)
{
	min = glm::min(min, other.aabb.min);
	max = glm::max(max, other.aabb.max);
	center = (min + max) / 2.0f;

	AABBLines.clear();
	ClearBuffer();
	CreateAABBLines();
	CreateBuffer();

}

void AABB_BV::Expand(std::vector<BoundingVolume> others)
{
	for (auto bv : others)
	{
		min = glm::min(min, bv.aabb.min);
		max = glm::max(max, bv.aabb.max);
	}
	center = (min + max) / 2.0f;

	AABBLines.clear();
	ClearBuffer();
	CreateAABBLines();
	CreateBuffer();
}

float AABB_BV::Volume()
{
	float dx = max.x - min.x;
	float dy = max.y - min.y;
	float dz = max.z - min.z;

	return dx * dy * dz;
}

void AABB_BV::Draw()
{
	if (VAO == 0 || VBO == 0)
		return;

	if (AABBLines.empty())
		return;

	auto shader_ = SHADERS->Get("Line");
	shader_->Use();
	shader_->Set("model", glm::mat4(1.0f));
	shader_->Set("color", lineColor);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(AABBLines.size()));
	glBindVertexArray(0);
}

void AABB_BV::Clear()
{
	ClearBuffer();

	AABBLines.clear();
	min = glm::vec3(FLT_MAX);
	max = glm::vec3(-FLT_MAX);
	center = glm::vec3(0);
}

void AABB_BV::CreateBuffer()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// vertex normal line
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * AABBLines.size()), AABBLines.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void AABB_BV::ClearBuffer()
{
	if (VAO == 0 || VBO == 0)
		return;

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	VAO = 0;
	VBO = 0;
}

void AABB_BV::CreateAABBLines()
{
	// top surface vertex
	glm::vec3 v1 = glm::vec3(min.x, max.y, max.z);
	glm::vec3 v2 = glm::vec3(max.x, max.y, max.z);
	glm::vec3 v3 = glm::vec3(max.x, max.y, min.z);
	glm::vec3 v4 = glm::vec3(min.x, max.y, min.z);

	// bottom surface vertex
	glm::vec3 v5 = glm::vec3(min.x, min.y, max.z);
	glm::vec3 v6 = glm::vec3(max.x, min.y, max.z);
	glm::vec3 v7 = glm::vec3(max.x, min.y, min.z);
	glm::vec3 v8 = glm::vec3(min.x, min.y, min.z);

	AABBLines.insert(AABBLines.end(), { v1, v2 });
	AABBLines.insert(AABBLines.end(), { v2, v3 });
	AABBLines.insert(AABBLines.end(), { v3, v4 });
	AABBLines.insert(AABBLines.end(), { v4, v1 });

	AABBLines.insert(AABBLines.end(), { v5, v6 });
	AABBLines.insert(AABBLines.end(), { v6, v7 });
	AABBLines.insert(AABBLines.end(), { v7, v8 });
	AABBLines.insert(AABBLines.end(), { v8, v5 });

	AABBLines.insert(AABBLines.end(), { v1, v5 });
	AABBLines.insert(AABBLines.end(), { v2, v6 });
	AABBLines.insert(AABBLines.end(), { v3, v7 });
	AABBLines.insert(AABBLines.end(), { v4, v8 });
}
