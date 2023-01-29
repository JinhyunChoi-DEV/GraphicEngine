#pragma once
#include <glm/vec3.hpp>

#include "Mesh.h"

// Only have Emissive Color Mesh
class StaticMesh : public Mesh
{
public:
	StaticMesh(std::vector<glm::vec3> vertex_, std::vector<unsigned int> indices_);
	StaticMesh(const StaticMesh& copy);
	~StaticMesh();

	void CopyData(const Mesh& copy) override;
	void Draw(Transform* transform) override;
	void DrawDebug(Transform* transform) override {}

private:
	void ClearBuffer();
	void CreateBuffer();

	std::vector<unsigned int> indices;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};
