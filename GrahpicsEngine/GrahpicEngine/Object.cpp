#include "Object.h"
#include "Transform.h"

Object::Object()
{
	SettingDefault("Object");
}

Object::Object(std::string name)
{
	SettingDefault(name);
}

Object::~Object()
{
	for (auto comp : components)
		delete comp;

	components.clear();
}

void Object::Update()
{
	if (!IsActive)
		return;

	if (transform->Rotation.x > 360)
		transform->Rotation.x -= 360;

	if (transform->Rotation.y > 360)
		transform->Rotation.y -= 360;

	if (transform->Rotation.z > 360)
		transform->Rotation.z -= 360;

	if (transform->Rotation.x < 0)
		transform->Rotation.x += 360;

	if (transform->Rotation.y < 0)
		transform->Rotation.y += 360;

	if (transform->Rotation.z < 0)
		transform->Rotation.z += 360;

	for (auto const& component : components)
		component->Update();
}

void Object::AddComponent(IComponent* component)
{
	component->Initialize();

	components.push_back(component);
}

void Object::DeleteComponent(IComponent* component)
{
	for (auto it = components.begin(); it != components.end();)
	{
		if (*it == component)
		{
			it = components.erase(it);
			return;
		}

		++it;
	}
}

void Object::SetActive(bool isActive)
{
	IsActive = isActive;
}

void Object::SettingDefault(const std::string& name)
{
	IsActive = true;
	Name = name;
	transform = new Transform();
}


