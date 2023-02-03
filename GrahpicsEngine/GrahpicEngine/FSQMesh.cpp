/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FSQMesh.cpp
Purpose: Making of implementing for FSQMesh.
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#include <glad/glad.h>

#include "FSQMesh.h"
#include "Camera.h"
#include "Graphic.h"
#include "Object.h"
#include "Shader.h"

FSQMesh::FSQMesh()
{
	vertex = {
		{-1,1,0},
		{-1,-1,0},
		{1,1,0},
		{1,-1,0}
	};

	uv =
	{
		{0,1},
		{0,0},
		{1,1,},
		{1,0}
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertex.size(), vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uv.size(), uv.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

FSQMesh::~FSQMesh()
{
	uv.clear();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, VBO);
}

void FSQMesh::Draw(Transform* transform)
{
	Transform trs;
	if (transform != nullptr)
		trs = *transform;
	else
		trs = baseTransform;

	shader->Use();
	shader->Set("model", trs.GetTransform());
	shader->Set("cameraPosition", GRAPHIC->MainCamera()->gameObject->transform->Position);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (int)vertex.size());
	glBindVertexArray(0);
}

void FSQMesh::DrawDebug(Transform* transform) {}
