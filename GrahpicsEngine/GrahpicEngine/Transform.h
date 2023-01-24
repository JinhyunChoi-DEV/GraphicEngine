#pragma once

#include <glm/glm.hpp>

class Transform
{
public:
	Transform();

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 Rotation;

	const glm::mat4 GetTransform();
private:
};