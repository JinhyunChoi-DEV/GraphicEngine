#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Camera.h"
#include "Graphic.h"
#include "Object.h"
#include "Transform.h"

Camera::Camera(Object* obj)
{
	gameObject = obj;

	Near = 0.1f;
	Far = 100.f;
	Fov = 45.0f;
	width = 800;
	height = 600;
	ratio = width / height;

	IsMain = false;
	UseConstantRatio = false;
	Position = gameObject->transform->Position;

	back = glm::vec3(0, 0, 1);
	right = glm::cross(-back, glm::vec3{ 0,1,0 });
	up = glm::cross(back, right);

	baseBack = back;
	baseRight = right;
}

Camera::~Camera()
{
	GRAPHIC->DeleteCamera(this);
}

void Camera::Initialize()
{
	GRAPHIC->AddCamera(this);
	width = GRAPHIC->ScreenSize.x;
	height = GRAPHIC->ScreenSize.y;
}

void Camera::Update()
{
	// Update Viewport Size
	width = GRAPHIC->ScreenSize.x;
	height = GRAPHIC->ScreenSize.y;

	// Update Ratio Values
	if (UseConstantRatio)
		ratio = 1.0f;
	else
		ratio = width / height;

	//Update Camera Position and Rotation
	auto rotation = gameObject->transform->Rotation;
	float xAngle = glm::radians(rotation.x);
	float yAngle = glm::radians(rotation.y);
	float zAngle = glm::radians(rotation.z);
	glm::vec3 rotationRadians = glm::vec3(xAngle, yAngle, zAngle);
	glm::quat rotateQuat = glm::quat(rotationRadians);
	glm::mat4 rotateMatrix = glm::toMat4(rotateQuat);

	back = rotateMatrix * glm::vec4(baseBack, 0);
	right = rotateMatrix * glm::vec4(baseRight, 0);
	up = glm::cross(back, right);

	gameObject->transform->Position = Position;
}

void Camera::Delete()
{
}

void Camera::Move(float speed, CameraMoveAxis axis)
{
	if (axis == CameraMoveAxis::Front)
		Position = Position - (back * speed);
	if (axis == CameraMoveAxis::Back)
		Position = Position + (back * speed);
	if (axis == CameraMoveAxis::Right)
		Position = Position + (right * speed);
	if (axis == CameraMoveAxis::Left)
		Position = Position - (right * speed);
}

const glm::mat4 Camera::Projection()
{
	auto fov = glm::radians(Fov);
	return glm::perspective(fov, ratio, Near, Far);
}

const glm::mat4 Camera::View()
{
	auto look = -back;
	return glm::lookAt(Position, Position + look, up);
}
