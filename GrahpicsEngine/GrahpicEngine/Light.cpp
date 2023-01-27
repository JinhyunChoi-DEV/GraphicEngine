#include "Light.h"
#include "Graphic.h"
#include "Object.h"


Light::Light(Object* root_)
{
	root = root_;

	Direction = glm::vec3(0);
	Ambient = glm::vec3(1);
	Diffuse = glm::vec3(1);
	Specular = glm::vec3(1);
	Type = LightType::Directional;
	InnerAngle = 0.0f;
	OuterAngle = 0.0f;
	Fallout = 1.0f;
	transform = root_->transform;
	IsActive = root_->IsActive;
}

Light::~Light()
{
	GRAPHIC->DeleteLight(this);
}

void Light::Initialize()
{
	GRAPHIC->AddLight(this);
}

void Light::Update()
{
	IsActive = root->IsActive;
}

void Light::Delete()
{
}
