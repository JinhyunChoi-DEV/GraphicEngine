#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "IComponent.h"

class Mesh : public IComponent
{
public:
	Mesh(std::vector<glm::vec3> vertex_, std::vector<glm::vec3> normal_,
		std::vector<glm::vec2> uv_, std::vector<unsigned int> indices_);

	void Initialize() override;
	void Update() override;
	void Delete() override;

	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> vertexNormal;
	std::vector<glm::vec3> faceNormal;
	std::vector<glm::vec3> vertexNormalLine;
	std::vector<glm::vec3> faceNormalLine;
	std::vector<glm::vec2> textureCoordinate;
	std::vector<unsigned int> indices;

	bool UseDeferredRendering;
	std::string Name;
	unsigned int VAO;
	unsigned int VBO[3];
	unsigned int EBO;
	// Shader?

private:
	void BindBuffer(int index);
	void CreateFaceNormal();
	void CreateSphericalUV();
};