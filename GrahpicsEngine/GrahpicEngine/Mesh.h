#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Material.h"

class Transform;
class Shader;

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	virtual void Draw(Transform* transform) = 0;
	virtual void DrawDebug(Transform* transform) = 0;

	bool UseDeferredRendering;
	Shader* shader;
	Material* material;

protected:
	std::vector<glm::vec3> vertex;
};

inline Mesh::Mesh()
{
	UseDeferredRendering = false;
	material = new Material();
}

inline Mesh::~Mesh()
{
	vertex.clear();
}
