#pragma once

#include <typeinfo>
#include <vector>
#include <string>
#include "IComponent.h"

class Transform;

class Object
{
public:
	Object();
	Object(std::string name);
	~Object();

	void Update();
	void AddComponent(IComponent* component);
	void DeleteComponent(IComponent* component);
	void SetActive(bool isActive);

	template <typename COMPONENT>
	COMPONENT* GetComponent();

	Transform* transform;
	bool IsActive;
	std::string Name;
private:
	void SettingDefault(const std::string& name);
	std::vector<IComponent*> components;
};

template <typename COMPONENT>
COMPONENT* Object::GetComponent()
{
	for (auto component : components)
	{
		if (typeid(COMPONENT) == typeid(*component))
			return dynamic_cast<COMPONENT*>(component);
	}

	return nullptr;
}
