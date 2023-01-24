#include <glm/gtx/quaternion.hpp>

#include "Transform.h"

Transform::Transform()
{
	Position = glm::vec3{ 0,0,0 };
	Scale = glm::vec3{ 1,1,1 };
	Rotation = glm::vec3{ 0,0,0 };
}

const glm::mat4 Transform::GetTransform()
{
	auto rotateQuat = glm::quat(Rotation);

	glm::mat4 identicalMatrix = glm::mat4(1.0f);
	glm::mat4 positionMatrix = glm::translate(identicalMatrix, Position);
	glm::mat4 scaleMatrix = glm::scale(identicalMatrix, Scale);
	glm::mat4 rotateMatrix = glm::toMat4(rotateQuat);

	return positionMatrix * rotateMatrix * scaleMatrix;
}