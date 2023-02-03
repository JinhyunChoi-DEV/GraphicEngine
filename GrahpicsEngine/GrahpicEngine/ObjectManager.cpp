/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectManager.cpp
Purpose: Making of implementing for ObjectManager System to easily make and delete objects
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#include "ObjectManager.h"
#include "Object.h"

ObjectManage* OBJMANAGER = nullptr;
ObjectManage::ObjectManage()
{
	if (OBJMANAGER == nullptr)
		OBJMANAGER = this;
}

ObjectManage::~ObjectManage()
{
	if (static_cast<int>(objects.size()) > 0)
		objects.clear();
}

void ObjectManage::Initialize()
{
}

void ObjectManage::Update()
{
	for (auto const& object : objects)
	{
		auto obj = object.second.get();
		obj->Update();
	}
}

void ObjectManage::Close()
{
	objects.clear();
}

void ObjectManage::Add(Object* object)
{
	auto name = object->Name;
	objects.insert(std::make_pair(name, object));
}

void ObjectManage::Delete(std::string name)
{
	if (!IsExist(name))
		return;

	objects.erase(name);
}

Object* ObjectManage::Get(std::string name)
{
	if (!IsExist(name))
		return nullptr;

	return objects[name].get();
}

std::vector<Object*> ObjectManage::GetAll() const
{
	std::vector<Object*> result;

	for (auto const& object : objects)
	{
		auto obj = object.second.get();
		result.push_back(obj);
	}

	return result;
}

bool ObjectManage::IsExist(std::string name)
{
	return objects.count(name) > 0;
}
