#pragma once
#include <glm/vec3.hpp>

#include "Mesh.h"

// Only have Emissive Color Mesh
class StaticMesh : public Mesh
{
public:
	StaticMesh(std::vector<glm::vec3> vertex_, std::vector<int> indices_);
	~StaticMesh();

	void Draw(Transform* transform) override;

	glm::vec3 Color;
private:
	void CreateBuffer();

	std::vector<int> indices;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};
