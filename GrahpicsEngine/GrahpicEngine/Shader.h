#pragma once

#include <string>
#include <glm/glm.hpp>

//TODO: 추후에 구현
/*
enum class ShaderType
{
	None = 0, Vertex, Fragment, Common
};
*/

class Shader
{
public:
	Shader(std::string name, std::string vertex, std::string fragment);

	void Recompile();
	void Use();
	void Set(const std::string& name, int value) const;
	void Set(const std::string& name, unsigned int value) const;
	void Set(const std::string& name, bool value) const;
	void Set(const std::string& name, float value) const;
	void Set(const std::string& name, glm::vec3 value) const;
	void Set(const std::string& name, glm::vec4 value) const;
	void Set(const std::string& name, glm::mat4 value) const;

	unsigned int ProgramID = 0;
	std::string Name;

private:
	void Compile();

	const std::string filePath = "../shaders/";
	const std::string versionText = "#version 430 core\n";
	std::string vertex = "";
	std::string fragment = "";
};
