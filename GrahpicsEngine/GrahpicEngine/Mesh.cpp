#include <iostream>
#include <glad/glad.h>

#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec3> vertex_, std::vector<glm::vec3> normal_,
	std::vector<glm::vec2> uv_, std::vector<unsigned> indices_)
{
	vertex = vertex_;
	vertexNormal = normal_;
	textureCoordinate = uv_;
	indices = indices_;

	if (textureCoordinate.size() <= 0)
		CreateSphericalUV();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, &VBO[0]);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	for (int i = 0; i < 3; ++i)
		BindBuffer(i);

	glBindVertexArray(0);
}

void Mesh::Initialize()
{
	std::cout << "Mesh Init" << std::endl;
}

void Mesh::Update()
{

}

void Mesh::Delete()
{

}

void Mesh::BindBuffer(int index)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);

	// 1: vertex, 2: normal, 3: textureCoordinate
	switch (index)
	{
	case 1:
	{
		glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * vertex.size()), vertex.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);
	}
	break;

	case 2:
	{
		glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * faceNormal.size()), faceNormal.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);
	}
	break;

	case 3:
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
