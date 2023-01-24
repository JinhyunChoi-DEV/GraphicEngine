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
