#pragma once
#include <unordered_map>

class Shader;

class ShaderManager
{
public:
	ShaderManager();

	void Compile(std::string name, std::string vertex, std::string fragment);
	void Recompile(std::string name);
	void Use(std::string name);

	Shader* Get(std::string name);

private:
	bool IsExist(std::string name);
	std::unordered_map<std::string, Shader*> shaders;
};

extern ShaderManager* SHADERS;
