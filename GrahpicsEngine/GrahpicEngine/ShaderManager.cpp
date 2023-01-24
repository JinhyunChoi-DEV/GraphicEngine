#include <iostream>
#include <cassert>

#include "ShaderManager.h"
#include "Shader.h"

ShaderManager* SHADERS = nullptr;

ShaderManager::ShaderManager()
{
	if (SHADERS == nullptr)
		SHADERS = this;
}

ShaderManager::~ShaderManager()
{
	shaders.clear();
}

void ShaderManager::Compile(std::string name, std::string vertex, std::string fragment)
{
	if (IsExist(name))
	{
		Recompile(name);
		return;
	}

	Shader* shader = new Shader(name, vertex, fragment);
	shaders.insert(std::make_pair(name, shader));
}

void ShaderManager::Recompile(std::string name)
{
	auto shader = Get(name);

	if (shader == nullptr)
	{
		std::cerr << "Try to compile undefined shader" << std::endl;
		assert(false);
		return;
	}

	shader->Recompile();
}

void ShaderManager::Use(std::string name)
{
	auto shader = Get(name);

	if (shader == nullptr)
	{
		std::cerr << "Try to use undefined shader" << std::endl;
		assert(false);
		return;
	}

	shader->Use();
}

Shader* ShaderManager::Get(std::string name)
{
	if (!IsExist(name))
		return nullptr;

	return shaders[name].get();
}

bool ShaderManager::IsExist(std::string name)
{
	return shaders.count(name) > 0;
}
