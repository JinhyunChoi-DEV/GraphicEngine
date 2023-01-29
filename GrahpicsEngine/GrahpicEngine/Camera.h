#pragma once
#include <glm/glm.hpp>
#include "IComponent.h"

class Object;
class Transform;

enum class CameraMoveAxis
{
	Front, Back, Right, Left
};

class Camera : public IComponent
{
public:
	Camera(Object* obj);
	~Camera();

	void Initialize() override;
	void Update() override;
	void Delete() override;

	void Move(float speed, CameraMoveAxis axis);
	const glm::mat4 View();
	const glm::mat4 Projection();
	Object* gameObject;
	glm::vec3 Position;

	bool IsMain;
	bool UseConstantRatio;
	float Near, Far, Fov;
private:
	glm::vec3 back, up, right;
	glm::vec3 baseBack, baseRight;
	float ratio, width, height;
};