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

void Object::Update()
{
	if (!IsActive)
		return;

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


