#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "ShaderManager.h"
#include "Transform.h"

Mesh::Mesh(std::vector<glm::vec3> vertex_, std::vector<glm::vec3> normal_,
	std::vector<glm::vec2> uv_, std::vector<unsigned> indices_)
{
	vertex = vertex_;
	vertexNormal = normal_;
	textureCoordinate = uv_;
	indices = indices_;
	shader = SHADERS->Get("Default");

	if (textureCoordinate.size() <= 0)
		CreateSphericalUV();

	CreateBuffers();
}

Mesh::Mesh(Mesh& copy)
{
	vertex = copy.vertex;
	vertexNormal = copy.vertexNormal;
	textureCoordinate = copy.textureCoordinate;
	indices = copy.indices;
	shader = SHADERS->Get("Default");

	CreateBuffers();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, &VBO[0]);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(Transform* transform)
{
	shader->Use();
	shader->Set("model", transform->GetTransform());

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Mesh::Initialize()
{
}

void Mesh::Update()
{
	//DO not call Draw Function, I want to explicitly call Draw function at Graphics.cpp - Update()
}

void Mesh::Delete()
{

}

void Mesh::CreateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, &VBO[0]);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	for (int i = 0; i < 3; ++i)
		BindBuffer(i);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Mesh::BindBuffer(int index)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);

	// 1: vertex, 2: normal, 3: textureCoordinate
	switch (index)
	{
	case 0:
	{
		glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * vertex.size()), vertex.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);
	}
	break;

	case 1:
	{
		glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * faceNormal.size()), faceNormal.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);
	}
	break;

	case 2:
	{
		glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec2) * textureCoordinate.size()), textureCoordinate.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(2);
	}
	break;

	default:
		break;
	}
}

void Mesh::CreateFaceNormal()
{
	//TODO:
	/*for (auto normal : vertexNormal)
	{
		auto index = vn
	}*/
}

void Mesh::CreateSphericalUV()
{
	for (auto position : vertex)
	{
		float x = position.x;
		float y = position.y;
		float z = position.z;

		float r = sqrt(x * x + y * y + z * z);
		// atan2 -> return -pi to pi
		float theta = glm::degrees(atan2(z, x)) + 180.f;
		float phi = acos(y / r);

		float u = theta / 360.0f;
		float v = 1.0f - (phi / 180.0f);

		textureCoordinate.push_back(glm::vec2(u, v));
	}
}
