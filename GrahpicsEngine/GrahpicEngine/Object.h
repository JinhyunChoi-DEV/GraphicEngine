/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Object.h
Purpose: Making of header file for Object data and it contains components, transform and mesh
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once

#include <typeinfo>
#include <vector>
#include <string>
#include "IComponent.h"
#include <iostream>

class Transform;
class Mesh;

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

	Transform* transform = nullptr;
	Mesh* mesh = nullptr;
	bool IsActive = false;
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