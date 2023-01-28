#pragma once
#include <glm/glm.hpp>
#include "Texture.h"

class Shader;

class Material
{
public:
	Material();

	void Update(Shader*);

	Texture AmbientTexture;
	Texture DiffuseTexture;
	Texture SpecularTexture;

	glm::vec3 AmbientColor;
	glm::vec3 DiffuseColor;
	glm::vec3 SpecularColor;
	glm::vec3 Intensity;

	bool UseGpuUV;
private:
	Texture baseDefault;

};