#pragma once
#include <unordered_map>
#include <memory>

class Object;

class ObjectManage
{
public:
	ObjectManage();

	void Initialize();
	void Update();
	void Close();

	void Add(Object* object);
	void Delete(std::string name);

	Object* Get(std::string name);
	std::vector<const Object*> GetAll();

private:
	bool IsExist(std::string name);
	std::unordered_map<std::string, std::unique_ptr<Object>> objects;
};
extern ObjectManage* OBJMANAGER;