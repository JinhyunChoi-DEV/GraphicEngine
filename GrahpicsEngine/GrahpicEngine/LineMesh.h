#pragma once
#include <glm/glm.hpp>

#include "Mesh.h"

class LineMesh : public Mesh
{
public:
	LineMesh(std::vector<glm::vec3> vertex_);
	~LineMesh();

	void Draw(Transform* transform) override;
	void DrawDebug(Transform* transform) override {}

	glm::vec3 Color;
private:
	void CreateBuffer();

	unsigned int VAO;
	unsigned int VBO;
};
