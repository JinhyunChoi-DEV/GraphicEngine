#include <glad/glad.h>

#include "LarssonSphere_BV.h"
#include "BoundingVolume.h"
#include "Mesh.h"
#include "Object.h"
#include "ShaderManager.h"
#include "Shader.h"

LarssonSphere_BV::LarssonSphere_BV()
{
	lineColor = glm::vec3(0, 0, 1);
}

void LarssonSphere_BV::CreateByMesh(Mesh* mesh)
{
	auto vertexSize = (int)mesh->Vertex().size();

	//Create Extreme Points
	auto result = ExtremePoint(mesh, glm::vec3(1, 0, 0));
	extremePoints.insert(extremePoints.end(), { std::get<0>(result), std::get<1>(result) });
	result = ExtremePoint(mesh, glm::vec3(0, 1, 0));
	extremePoints.insert(extremePoints.end(), { std::get<0>(result), std::get<1>(result) });
	result = ExtremePoint(mesh, glm::vec3(0, 0, 1));
	extremePoints.insert(extremePoints.end(), { std::get<0>(result), std::get<1>(result) });

	float d = sqrtf(1.0f / 3.0f);
	result = ExtremePoint(mesh, glm::vec3(d, d, d));
	extremePoints.insert(extremePoints.end(), { std::get<0>(result), std::get<1>(result) });
	result = ExtremePoint(mesh, glm::vec3(d, -d, d));
	extremePoints.insert(extremePoints.end(), { std::get<0>(result), std::get<1>(result) });
	result = ExtremePoint(mesh, glm::vec3(d, -d, -d));
	extremePoints.insert(extremePoints.end(), { std::get<0>(result), std::get<1>(result) });
	result = ExtremePoint(mesh, glm::vec3(d, -d, -d));
	extremePoints.insert(extremePoints.end(), { std::get<0>(result), std::get<1>(result) });

	//Create Sphere by using extreme Points
	int size = (int)extremePoints.size();
	for (int i = 0; i < size; ++i)
	{
		auto baseP = extremePoints[i];
		for (int j = i; j < size; ++j)
		{
			auto currentP = extremePoints[j];
			float distance = glm::distance(baseP, currentP);
			float radius = distance / 2.0f;
			if (extreme.radius < radius || std::isinf(extreme.radius))
			{
				extreme.center = (baseP + currentP) / 2.0f;
				extreme.radius = radius;
			}
		}
	}

	// Grow Sphere
	if (vertexSize > (int)(extremePoints.size() / 2))
		GrowSphere(extreme, mesh);

	//Create ExtremeData
	CreateSphere();

	CreateBuffer();
}

// Sphere can not expand by Minimum and Maximum points
void LarssonSphere_BV::Expand(glm::vec3 min_, glm::vec3 max_)
{}

void LarssonSphere_BV::Expand(BoundingVolume other)
{
	if (std::isinf(extreme.radius))
	{
		extreme = other.larssonSphere.extreme;
	}
	else
	{
		auto otherExt = other.larssonSphere.extreme;
		glm::vec3 newCenter = (extreme.center + otherExt.center) / 2.0f;
		float centerDistance = glm::distance(extreme.center, otherExt.center);
		float diameter = centerDistance + extreme.radius + otherExt.radius;
		float newRadius = diameter / 2.0f;

		extreme.center = newCenter;
		extreme.radius = newRadius;
	}

	vertices.clear();
	lineIndices.clear();

	ClearBuffer();
	CreateSphere();
	CreateBuffer();
}

void LarssonSphere_BV::Expand(std::vector<BoundingVolume> others)
{
	glm::vec3 center(0);
	for (auto bv : others)
	{
		center += bv.larssonSphere.extreme.center;
	}
	center /= others.size();

	float r = 0;
	for (int i = 0; i < (int)others.size(); ++i)
	{
		auto otherExt = others[i].larssonSphere.extreme;
		float centerDistance = glm::distance(otherExt.center, center);
		r = glm::max(r, centerDistance + otherExt.radius);
	}
	extreme.center = center;
	extreme.radius = r;

	vertices.clear();
	lineIndices.clear();

	ClearBuffer();
	CreateSphere();
	CreateBuffer();
}

float LarssonSphere_BV::Volume()
{
	float cube_radius = powf(extreme.radius, 3);
	return (4.0f / 3.0f) * glm::pi<float>() * cube_radius;
}

void LarssonSphere_BV::Draw()
{
	if (VAO == 0 || VBO == 0)
		return;

	if (vertices.empty())
		return;

	if (lineIndices.empty())
		return;

	auto shader_ = SHADERS->Get("Line");
	shader_->Use();
	shader_->Set("model", glm::mat4(1.0f));
	shader_->Set("color", lineColor);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, (GLsizei)lineIndices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void LarssonSphere_BV::Clear()
{
	ClearBuffer();

	vertices.clear();
	lineIndices.clear();
	min = glm::vec3(FLT_MAX);
	max = glm::vec3(-FLT_MAX);
}

void LarssonSphere_BV::CreateSphere()
{
	auto data = CreateBoundingSphere(extreme, 37, 37);
	vertices = std::get<0>(data);
	lineIndices = std::get<1>(data);

	center = extreme.center;
	min = extreme.center - extreme.radius;
	max = extreme.center + extreme.radius;
}

void LarssonSphere_BV::CreateBuffer()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * vertices.size()), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lineIndices.size() * sizeof(unsigned int), lineIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void LarssonSphere_BV::ClearBuffer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	VAO = 0;
	VBO = 0;
	EBO = 0;
}
