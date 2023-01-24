#include "ObjectManager.h"
#include "Object.h"

ObjectManage* OBJMANAGER = nullptr;
ObjectManage::ObjectManage()
{
	if (OBJMANAGER == nullptr)
		OBJMANAGER = this;
}

void ObjectManage::Initialize()
{
	objects.clear();
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

std::vector<const Object*> ObjectManage::GetAll()
{
	std::vector<const Object*> result;

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
