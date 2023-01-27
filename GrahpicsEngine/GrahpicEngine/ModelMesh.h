#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Mesh.h"

class Transform;

class ModelMesh : public Mesh
{
public:
	ModelMesh(std::vector<glm::vec3> vertex_, std::vector<glm::vec3> normal_,
		std::vector<glm::vec2> uv_, std::vector<unsigned int> indices_);
	ModelMesh(ModelMesh& copy);
	~ModelMesh();

	void Draw(Transform* transform) override;
	void DrawDebug(Transform* transform) override;

	bool DrawVertexNormal;
	bool DrawFaceNormal;
private:
	void DrawModel(Transform*);
	void DrawNormalLine(Transform*);

	void CreateModelBuffers();
	void CreateLineBuffers();
	void CreateFaceNormal();
	void CreateSphericalUV();
	void CreateNormalLines();

	std::vector<glm::vec3> vertexNormal;
	std::vector<glm::vec3> faceNormal;
	std::vector<glm::vec2> textureCoordinate;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertexNormalLine;
	std::vector<glm::vec3> faceNormalLine;

	unsigned int VAO;
	unsigned int VBO[3];
	unsigned int EBO;
	unsigned int lineVAO[2];
	unsigned int lineVBO[2];
};