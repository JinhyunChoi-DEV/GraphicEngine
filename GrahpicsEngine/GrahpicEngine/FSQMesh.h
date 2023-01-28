#pragma once
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Transform.h"

// Full Screen Quad
class FSQMesh : public Mesh
{
public:
	FSQMesh();
	~FSQMesh();

	void Draw(Transform* transform) override;
	void DrawDebug(Transform* transform) override;

private:
	Transform baseTransform;
	std::vector<glm::vec2> uv;
	unsigned int VAO;
	unsigned int VBO[2];
};