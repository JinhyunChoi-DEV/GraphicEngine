#pragma once
#include <glm/glm.hpp>
#include "IComponent.h"

enum class LightType
{
	Directional = 0, Point, Spotlight, Count
};

class Object;
class Transform;

class Light : public IComponent
{
public:
	Light(Object*);
	~Light();

	void Initialize() override;
	void Update() override;
	void Delete() override;

	Transform* transform;
	LightType Type;
	glm::vec3 Direction;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float InnerAngle;
	float OuterAngle;
	float Fallout;

private:
	Object* root;
};
