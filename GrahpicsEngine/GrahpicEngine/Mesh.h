#pragma once
#include <string>
#include <vector>

class Transform;
class Shader;

class Mesh
{
public:
	virtual ~Mesh();

	virtual void Draw(Transform* transform) = 0;

	bool UseDeferredRendering;
	std::string Name;
	Shader* shader;
	Mesh* childMesh;

protected:
	std::vector<glm::vec3> vertex;
};

inline Mesh::~Mesh()
{
	vertex.clear();
}