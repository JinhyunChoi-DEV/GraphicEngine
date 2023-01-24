#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

#include "Shader.h"

namespace
{
	std::string GetShaderCode(std::string filePath);
	GLuint CompileShader(std::string name, std::string versionCode, std::string shaderCode, GLenum shaderType);
	GLuint LinkShader(GLuint vertex_handle, GLuint fragment_handle);
}

Shader::Shader(std::string name, std::string vertex, std::string fragment)
{
	this->vertex = vertex;
	this->fragment = fragment;
	Name = name;

	Compile();
}

Shader::~Shader()
{
	if (ProgramID != 0)
	{
		glDeleteProgram(ProgramID);
	}
}

void Shader::Recompile()
{
	if (ProgramID != 0)
	{
		glDeleteProgram(ProgramID);
		ProgramID = 0;
	}

	if (vertex.size() == 0 || fragment.size() == 0 || Name.size() == 0)
		return;

	Compile();
}

void Shader::Set(const std::string& name, bool value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation => " << name << std::endl;
		return;
	}

	glUniform1i(location, static_cast<int>(value));
}

void Shader::Use()
{
	glUseProgram(ProgramID);
}

void Shader::Set(const std::string& name, int value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniform1i(location, value);
}

void Shader::Set(const std::string& name, unsigned int value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniform1ui(location, value);
}

void Shader::Set(const std::string& name, float value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniform1f(location, value);
}

void Shader::Set(const std::string& name, glm::vec3 value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniform3fv(location, 1, &value[0]);
}

void Shader::Set(const std::string& name, glm::vec4 value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniform4fv(location, 1, &value[0]);
}

void Shader::Set(const std::string& name, glm::mat4 value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void Shader::Compile()
{
	auto vertexCodePath = filePath + vertex;
	auto fragCodePath = filePath + fragment;
	unsigned vertexShader = CompileShader(Name, versionText, vertexCodePath, GL_VERTEX_SHADER);
	unsigned fragmentShader = CompileShader(Name, versionText, fragCodePath, GL_FRAGMENT_SHADER);

	ProgramID = LinkShader(vertexShader, fragmentShader);

	glDeleteProgram(vertexShader);
	glDeleteProgram(fragmentShader);
}

namespace
{
	std::string GetShaderCode(std::string filePath)
	{
		std::ifstream file;
		std::stringstream fileStream;

		file.open(filePath);
		fileStream << file.rdbuf();
		file.close();

		return fileStream.str();
	}

	GLuint CompileShader(std::string name, std::string versionCode, std::string filePath, GLenum shaderType)
	{
		GLuint shader = 0;
		GLint compile_result = GL_FALSE;

		shader = glCreateShader(shaderType);

		std::string tempCode = versionCode + GetShaderCode(filePath);
		const char* code = tempCode.c_str();
		std::cout << code << std::endl;
		glShaderSource(shader, 1, &code, nullptr);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);
		if (compile_result == GL_FALSE)
		{
			std::string error_log = "Failed to compile: " + name;
			if (shaderType == GL_FRAGMENT_SHADER)
				error_log += " fragment shader:\n";
			else if (shaderType == GL_VERTEX_SHADER)
				error_log += " vertex shader:\n";
			GLint log_length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

			GLchar* log_message = new GLchar[log_length];
			glGetShaderInfoLog(shader, log_length, nullptr, log_message);

			std::cerr << error_log << log_message << "\n";
			assert(false);
			delete[] log_message;
		}
		return shader;
	}

	GLuint LinkShader(GLuint vertex_handle, GLuint fragment_handle)
	{
		GLint link_result = GL_FALSE;
		GLuint program = 0;
		program = glCreateProgram();

		glAttachShader(program, vertex_handle);
		glAttachShader(program, fragment_handle);

		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &link_result);

		if (link_result == GL_FALSE)
		{
			GLint log_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

			GLchar* log_message = new GLchar[log_length];
			glGetProgramInfoLog(program, log_length, nullptr, log_message);

			std::cerr << "shader program failed to link\n" << log_message << "\n";
			assert(false);
			delete[] log_message;
		}
		return program;
	}
}
