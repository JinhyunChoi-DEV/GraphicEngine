#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Shader.h"
#include "IComponent.h"

class Transform;

class Mesh : public IComponent
{
public:
	Mesh(std::vector<glm::vec3> vertex_, std::vector<glm::vec3> normal_,
		std::vector<glm::vec2> uv_, std::vector<unsigned int> indices_);
	Mesh(Mesh& copy);
	~Mesh();

	void Draw(Transform* transform);
	void DrawNormalLine(bool, bool, Transform*);

	void Initialize() override;
	void Update() override;
	void Delete() override;

	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> vertexNormal;
	std::vector<glm::vec3> faceNormal;
	std::vector<glm::vec2> textureCoordinate;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertexNormalLine;
	std::vector<glm::vec3> faceNormalLine;

	bool UseDeferredRendering;
	std::string Name;
	Shader* shader;

private:
	unsigned int VAO;
	unsigned int VBO[3];
	unsigned int EBO;
	unsigned int lineVAO[2];
	unsigned int lineVBO[2];

	void CreateModelBuffers();
	void CreateLineBuffers();
	void CreateFaceNormal();
	void CreateSphericalUV();
	void CreateNormalLines();
};