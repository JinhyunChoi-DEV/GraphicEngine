#pragma once
#include <glm/glm.hpp>
#include "IComponent.h"

class Object;
class Transform;

class Camera : public IComponent
{
public:
	Camera(Object* obj);
	~Camera();

	void Initialize() override;
	void Update() override;
	void Delete() override;

	const glm::mat4 View();
	const glm::mat4 Projection();

	bool IsMain;
	bool UseConstantRatio;
	float Near, Far, Fov;
private:
	Transform* transform;
	glm::vec3 position, back, up, right;
	float ratio, width, height;
};