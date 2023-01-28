#include <glad/glad.h>
#include "StaticMesh.h"

#include "Graphic.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Transform.h"

StaticMesh::StaticMesh(std::vector<glm::vec3> vertex_, std::vector<unsigned int> indices_)
{
	vertex = vertex_;
	indices = indices_;
	Color = glm::vec3(1, 1, 1);
	shader = SHADERS->Get("Default");

	CreateBuffer();
}

StaticMesh::~StaticMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void StaticMesh::Draw(Transform* transform)
{
	if (UseDeferredRendering)
	{
		auto currentShader = SHADERS->Get("GBuffer");
		currentShader->Use();
		currentShader->Set("model", transform->GetTransform());
		currentShader->Set("affectedLighting", true);
		material->Update(currentShader);
	}
	else
	{
		shader->Use();
		shader->Set("model", transform->GetTransform());
		shader->Set("color", Color);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void StaticMesh::CreateBuffer()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	// ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}