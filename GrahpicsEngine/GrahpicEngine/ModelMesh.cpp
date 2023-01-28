#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "ModelMesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Graphic.h"
#include "ShaderManager.h"
#include "Transform.h"

ModelMesh::ModelMesh(std::vector<glm::vec3> vertex_, std::vector<glm::vec3> normal_,
	std::vector<glm::vec2> uv_, std::vector<unsigned int> indices_)
{
	vertex = vertex_;
	vertexNormal = normal_;
	textureCoordinate = uv_;
	indices = indices_;
	shader = SHADERS->Get("Default");
	DrawFaceNormal = false;
	DrawVertexNormal = false;

	if (textureCoordinate.size() <= 0)
		CreateSphericalUV();

	CreateFaceNormal();
	CreateNormalLines();
	CreateModelBuffers();
	CreateLineBuffers();
}

ModelMesh::ModelMesh(ModelMesh& copy)
{
	vertex = copy.vertex;
	vertexNormal = copy.vertexNormal;
	faceNormal = copy.faceNormal;
	textureCoordinate = copy.textureCoordinate;
	indices = copy.indices;
	vertexNormalLine = copy.vertexNormalLine;
	faceNormalLine = copy.faceNormalLine;
	DrawFaceNormal = false;
	DrawVertexNormal = false;

	shader = copy.shader;

	CreateModelBuffers();
	CreateLineBuffers();
}

ModelMesh::~ModelMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, &VBO[0]);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(2, lineVAO);
	glDeleteBuffers(2, lineVBO);
}

void ModelMesh::Draw(Transform* transform)
{
	DrawModel(transform);
}

// for the assignment
void ModelMesh::DrawDebug(Transform* transform)
{
	DrawNormalLine(transform);
}

void ModelMesh::DrawModel(Transform* transform)
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
		shader->Set("cameraPosition", GRAPHIC->MainCamera()->Position);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void ModelMesh::DrawNormalLine(Transform* transform)
{
	if (vertexNormalLine.empty() || faceNormalLine.empty())
		return;

	auto shader_ = SHADERS->Get("Line");
	shader_->Use();
	shader_->Set("model", transform->GetTransform());

	if (DrawVertexNormal)
	{
		shader_->Set("color", glm::vec3(0, 1, 0));
		glBindVertexArray(lineVAO[0]);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertexNormalLine.size()));
		glBindVertexArray(0);
	}

	if (DrawFaceNormal)
	{
		shader_->Set("color", glm::vec3(0, 0, 1));
		glBindVertexArray(lineVAO[1]);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(faceNormalLine.size()));
		glBindVertexArray(0);
	}
}

void ModelMesh::CreateModelBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	// normal
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexNormal.size() * sizeof(glm::vec3), vertexNormal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	// texture
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, textureCoordinate.size() * sizeof(glm::vec2), textureCoordinate.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);

	// ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ModelMesh::CreateFaceNormal()
{
	for (unsigned i = 0; i < indices.size(); i += 3)
	{
		auto first = vertex[i];
		auto second = vertex[i + 1];
		auto third = vertex[i + 2];

		auto v1 = second - first;
		auto v2 = third - first;

		auto normal = glm::normalize(glm::cross(v1, v2));

		for (int j = 0; j < 3; ++j)
			faceNormal.push_back(normal);
	}
}

void ModelMesh::CreateSphericalUV()
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

void ModelMesh::CreateNormalLines()
{
	for (auto i : indices)
	{
		auto v1 = vertex[i];
		auto v2 = v1 + (vertexNormal[i] * 0.15f);
		vertexNormalLine.insert(vertexNormalLine.end(), { v1,v2 });
	}

	// the Assimp add indices based on face, so i+=3 can do it
	for (unsigned i = 0; i < indices.size(); i += 3)
	{
		auto first = indices[i];
		auto second = indices[i + 1];
		auto third = indices[i + 2];

		auto v1 = (vertex[first] + vertex[second] + vertex[third]) / 3.0f;

		for (int j = 0; j < 3; ++j)
		{
			auto v2 = v1 + faceNormal[i + j] * 0.15f;
			faceNormalLine.insert(faceNormalLine.end(), { v1,v2 });
		}
	}
}

void ModelMesh::CreateLineBuffers()
{
	glGenVertexArrays(2, lineVAO);
	glGenBuffers(2, lineVBO);

	// vertex normal line
	glBindVertexArray(lineVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * vertexNormalLine.size()), vertexNormalLine.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// face normal line
	glBindVertexArray(lineVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * faceNormalLine.size()), faceNormalLine.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

