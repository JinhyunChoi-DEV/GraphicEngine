#include <glad/glad.h>
#include "LineMesh.h"

#include "Graphic.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Transform.h"

LineMesh::LineMesh(std::vector<glm::vec3> vertex_)
{
	vertex = vertex_;
	Color = glm::vec3(1, 1, 1);
	shader = SHADERS->Get("Line");

	CreateBuffer();
}

LineMesh::~LineMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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
		shader->Set("color", Color);
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
