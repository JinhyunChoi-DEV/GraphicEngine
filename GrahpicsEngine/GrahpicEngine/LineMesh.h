#pragma once
#include <glm/glm.hpp>

#include "Mesh.h"

class LineMesh : public Mesh
{
public:
	LineMesh(std::vector<glm::vec3> vertex_);
	~LineMesh();

	void CopyData(const Mesh& copy) override {}
	void Draw(Transform* transform) override;
	void DrawDebug(Transform* transform) override {}

private:
	void CreateBuffer();
	void ClearBuffer();

	unsigned int VAO;
	unsigned int VBO;
};
