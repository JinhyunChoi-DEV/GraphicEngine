/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: LineMesh.cpp
Purpose: Making of implementing for LineMesh
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#include <glad/glad.h>
#include "LineMesh.h"

#include "Graphic.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Transform.h"

LineMesh::LineMesh(std::vector<glm::vec3> vertex_)
{
	vertex = vertex_;
	shader = SHADERS->Get("Line");

	CreateBuffer();
}

LineMesh::~LineMesh()
{
	ClearBuffer();
}

void LineMesh::Draw(Transform* transform)
{
	if (UseDeferredRendering)
	{
		auto currentShader = SHADERS->Get("GBuffer");
		currentShader->Use();
		currentShader->Set("model", transform->GetTransform());
		currentShader->Set("affectedLighting", false);
		material->Update(currentShader);
	}
	else
	{
		shader->Use();
		shader->Set("model", transform->GetTransform());
		shader->Set("color", material->AmbientColor);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertex.size()));
	glBindVertexArray(0);

}

void LineMesh::CreateBuffer()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void LineMesh::ClearBuffer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
