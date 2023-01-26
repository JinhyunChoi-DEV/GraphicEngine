#include "Light.h"
#include "Graphic.h"


Light::Light(Transform* transform_)
{
	Direction = glm::vec3(0);
	Ambient = glm::vec3(1);
	Diffuse = glm::vec3(1);
	Specular = glm::vec3(1);
	Type = LightType::Directional;
	InnerAngle = 0.0f;
	OuterAngle = 0.0f;
	Fallout = 1.0f;
	transform = transform_;
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
}

void Light::Delete()
{
}
