#include <glad/glad.h>
#include "Material.h"
#include "Shader.h"

Material::Material()
{
	AmbientTexture = baseDefault;
	DiffuseTexture = baseDefault;
	SpecularTexture = baseDefault;

	AmbientColor = glm::vec3(0);
	DiffuseColor = glm::vec3(0);
	SpecularColor = glm::vec3(0);
	Intensity = glm::vec3(1);

	UseGpuUV = false;
}

void Material::Update(Shader* shader)
{
	// Ambient
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, AmbientTexture.BufferObject);
	shader->Set("ambientTexture", 10);
	shader->Set("useAmbientTexture", (int)AmbientTexture.BufferObject != -1);

	// Diffuse
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, DiffuseTexture.BufferObject);
	shader->Set("diffuseTexture", 11);
	shader->Set("useDiffuseTexture", (int)DiffuseTexture.BufferObject != -1);

	// Specular
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, SpecularTexture.BufferObject);
	shader->Set("specularTexture", 12);
	shader->Set("useSpecularTexture", (int)SpecularTexture.BufferObject != -1);

	shader->Set("ambientColor", AmbientColor);
	shader->Set("diffuseColor", DiffuseColor);
	shader->Set("specularColor", SpecularColor);
	shader->Set("useGpuUV", UseGpuUV);
}
