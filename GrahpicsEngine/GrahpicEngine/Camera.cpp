#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Graphic.h"
#include "Object.h"
#include "Transform.h"

Camera::Camera(Object* obj)
{
	transform = obj->transform;

	Near = 0.1f;
	Far = 100.f;
	Fov = 45.0f;
	width = 800;
	height = 600;
	ratio = width / height;

	IsMain = false;
	UseConstantRatio = false;
	Position = transform->Position;

	back = glm::vec3(0, 0, 1);
	right = glm::cross(-back, glm::vec3{ 0,1,0 });
	up = glm::cross(back, right);
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
	// Update Camera Position
	Position = transform->Position;

	// Update Viewport Size
	width = GRAPHIC->ScreenSize.x;
	height = GRAPHIC->ScreenSize.y;

	// Update Ratio Values
	if (UseConstantRatio)
		ratio = 1.0f;
	else
		ratio = width / height;
}

void Camera::Delete()
{
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
