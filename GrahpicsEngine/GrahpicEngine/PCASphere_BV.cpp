#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

#include "PCASphere_BV.h"
#include "BoundingVolume.h"
#include "Mesh.h"
#include "Object.h"
#include "Shader.h"
#include "ShaderManager.h"

PCASphere_BV::PCASphere_BV()
{
	lineColor = glm::vec3(1, 1, 0);
}

void PCASphere_BV::CreateByMesh(Mesh* mesh)
{
	// Doing PCA Stuff;
	CreateCovarianceMat(mesh);
	glm::mat3 A = covarianceMatrix;
	glm::mat3 V(1.0f);
	float prevSum = std::numeric_limits<float>::infinity();
	while (!CheckConvergence(A, prevSum))
	{
		FindMaximumElement(A);

		float beta = (A[q][q] - A[p][p]) / (2 * A[p][q]);
		float t = sin(beta) / (abs(beta) + sqrtf(beta * beta + 1));
		float c = 1.f / sqrtf(t * t + 1);
		float s = c * t;

		glm::mat3 J = GetJacobiMatrix(c, s);
		glm::mat3 inverse_J = glm::transpose(J);
		A = inverse_J * A * J;
		V = V * J;
	}

	glm::vec3 eigenVector = GetEigenVector(V);
	auto extremePoint = ExtremePoint(mesh, eigenVector);
	auto p1 = std::get<0>(extremePoint);
	auto p2 = std::get<1>(extremePoint);
	float distance = glm::distance(p1, p2);
	extreme.center = (p1 + p2) / 2.0f;
	extreme.radius = distance / 2.0f;

	// Grow Sphere for making enclose all vertex
	GrowSphere(extreme, mesh);

	// Set Sphere Data
	CreateSphere();
	CreateBuffer();
}

// Sphere can not expand by Minimum and Maximum points
void PCASphere_BV::Expand(glm::vec3 min_, glm::vec3 max_)
{}

void PCASphere_BV::Expand(BoundingVolume other)
{
	if (std::isinf(extreme.radius))
	{
		extreme = other.pcaSphere.extreme;
	}
	else
	{
		auto otherExt = other.pcaSphere.extreme;
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

void PCASphere_BV::Expand(std::vector<BoundingVolume> others)
{
	glm::vec3 center(0);
	for (auto bv : others)
	{
		center += bv.pcaSphere.center;
	}
	center /= others.size();

	float r = 0;
	for (int i = 0; i < (int)others.size(); ++i)
	{
		auto otherExt = others[i].pcaSphere.extreme;
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

float PCASphere_BV::Volume()
{
	float cube_radius = powf(extreme.radius, 3);
	return (4.0f / 3.0f) * glm::pi<float>() * cube_radius;
}

void PCASphere_BV::Draw()
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

void PCASphere_BV::Clear()
{
	ClearBuffer();

	vertices.clear();
	lineIndices.clear();
	min = glm::vec3(FLT_MAX);
	max = glm::vec3(-FLT_MAX);
}

void PCASphere_BV::CreateBuffer()
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

void PCASphere_BV::ClearBuffer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	VAO = 0;
	VBO = 0;
	EBO = 0;
}

void PCASphere_BV::CreateSphere()
{
	auto data = CreateBoundingSphere(extreme, 37, 37);
	vertices = std::get<0>(data);
	lineIndices = std::get<1>(data);

	center = extreme.center;
	min = extreme.center - extreme.radius;
	max = extreme.center + extreme.radius;
}

void PCASphere_BV::CreateCovarianceMat(Mesh* mesh)
{
	auto vertex = mesh->Vertex();

	// Create Mean
	glm::vec3 mean(0);
	for (auto v : vertex)
		mean += v;
	mean /= (int)vertex.size();

	// Calculate CovarianceMat
	glm::mat3 covariance(0);
	for (auto v : vertex)
	{
		float xy = (v.x - mean.x) * (v.y - mean.y);
		float xz = (v.x - mean.x) * (v.z - mean.z);
		float yz = (v.y - mean.y) * (v.z - mean.z);

		// base on X
		covariance[0][0] += powf(v.x - mean.x, 2);
		covariance[0][1] += xy;
		covariance[0][2] += xz;

		// base on Y
		covariance[1][0] += xy;
		covariance[1][1] += powf(v.y - mean.y, 2);
		covariance[1][2] += yz;

		// base on Z
		covariance[2][0] += xz;
		covariance[2][1] += yz;
		covariance[2][2] += powf(v.z - mean.z, 2);
	}
	covariance /= (int)vertex.size();
	covarianceMatrix = covariance;
}

bool PCASphere_BV::CheckConvergence(glm::mat3 mat, float& prevSum)
{
	float xRow = mat[0][1] + mat[0][2];
	float yRow = mat[1][0] + mat[1][2];
	float zRow = mat[2][0] + mat[2][1];
	float sum = xRow + yRow + zRow;

	// 0.0001f is epsilon
	if (std::isinf(prevSum))
	{
		prevSum = sum;
		return fabs(sum - 0.0f) < 0.0001f;
	}
	else
	{
		float tempPrevSum = prevSum;
		prevSum = sum;
		return fabs(sum - 0.0f) < 0.0001f || fabs(tempPrevSum - sum) < 0.0001f;
	}
}

void PCASphere_BV::FindMaximumElement(glm::mat3 mat)
{
	float maxElement = -FLT_MAX;
	p = 0;
	q = 0;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (i == j)
				continue;

			if (mat[i][j] > maxElement)
			{
				maxElement = abs(mat[i][j]);
				p = i;
				q = j;
			}
		}
	}
}

glm::mat3 PCASphere_BV::GetJacobiMatrix(float c, float s)
{
	glm::mat3 result = glm::identity<glm::mat3>();
	result[p][p] = c;
	result[p][q] = -s;
	result[q][p] = s;
	result[q][q] = c;

	return result;
}

glm::vec3 PCASphere_BV::GetEigenVector(glm::mat3 V)
{
	int maxIndex = 0;
	float maxElement = -FLT_MAX;
	for (int i = 0; i < 3; ++i)
	{
		if (abs(V[i][i]) > maxElement)
		{
			maxElement = abs(V[i][i]);
			maxIndex = i;
		}
	}

	return V[maxIndex];
}