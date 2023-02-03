/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectManager.h
Purpose: Making of header file for ObjectManager System to easily make and delete objects
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <unordered_map>
#include <memory>

class Object;

class ObjectManage
{
public:
	ObjectManage();
	~ObjectManage();

	void Initialize();
	void Update();
	void Close();

	void Add(Object* object);
	void Delete(std::string name);

	Object* Get(std::string name);
	std::vector<Object*> GetAll() const;

private:
	bool IsExist(std::string name);
	std::unordered_map<std::string, std::unique_ptr<Object>> objects;
};
extern ObjectManage* OBJMANAGER;