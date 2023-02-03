/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Phong Shading.frag
Purpose: Making of fragment shader for implementing Phong Shading
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

struct Light
{
	uint type;			// 0, Directional=0, Point=1, Spotlight=2
	vec3 position;		// 16
	vec3 direction;		// 32											
	vec3 ambient;		// 48
	vec3 diffuse;		// 64

	vec3 specular;		// 80
	float innerAngle;	// 92

	float outerAngle;	// 96
	float fallOut;		// 104
	/* 
	Total = type(16) + position(16) + 
	direction(16) + ambient(16) + diffuse(16) +
	specular(12) + innerAngle(4) + 
	outerAngle(8) + fallOut(8) = 112
	*/
};

layout(std140, binding = 1) uniform Lights
{
	uint lightCount;			// 4
	float near;					// 4 -> 8
	float far;					// 8 -> 16

	vec3 attenuationConstants;	// 16 -> 32
	vec3 globalAmbientColor;	// 16 -> 48
	vec3 fogColor;				// 16 -> 64
	Light light[16];		// 108 each one
} lights;

uniform vec3 cameraPosition;

in vec3 modelPosition;
in vec3 worldPosition;
in vec3 outputNormal;
in vec2 outputUV;

out vec4 outputColor;

float constStrenght = 0.5;
vec3 normal;
vec3 getLightDirection(in Light light);
vec3 getAmbient(in Light light);
vec3 getDiffuse(in Light light);
vec3 getSpecular(in Light light);
vec3 getColorWithLight(in vec3 ambient, in vec3 diffuse, in vec3 specular, in Light light);
void main()
{
	normal = normalize(outputNormal);
	float S = (lights.far - length(cameraPosition - worldPosition))/(lights.far - lights.near);

	vec3 lightsResult = vec3(0);
	for(int i =0; i<lights.lightCount; ++i)
	{
		vec3 ambient = getAmbient(lights.light[i]);
		vec3 diffuse = getDiffuse(lights.light[i]);
		vec3 specular = getSpecular(lights.light[i]);
		vec3 colorWithLight = getColorWithLight(ambient, diffuse, specular, lights.light[i]);

		lightsResult += max(colorWithLight, 0);
	}

	vec3 I_Local = lights.globalAmbientColor * constStrenght + lightsResult;
	vec3 colorResult = S*I_Local + (1.0-S)*lights.fogColor;
	outputColor = vec4(colorResult, 1.0);
}

vec3 getLightDirection(in Light light)
{
	if(light.type == 0)
		return normalize(-light.direction);
	else
		return normalize(light.position - worldPosition);
}

vec3 getAmbient(in Light light)
{
	return light.ambient * constStrenght;
}

vec3 getDiffuse(in Light light)
{
	vec3 lightDir = getLightDirection(light);
	float diff = max(dot(normal, lightDir), 0);

	return light.diffuse * diff;
}

vec3 getSpecular(in Light light)
{
	vec3 lightDir = getLightDirection(light);

	vec3 viewVector = normalize(cameraPosition - worldPosition);
	vec3 reflection = 2.0 * dot(normal, lightDir) * normal - lightDir;
	float spec = pow(max(dot(reflection, viewVector), 0), 32.0);
	return light.specular * spec;
}

vec3 getColorWithLight(in vec3 ambient, in vec3 diffuse, in vec3 specular, in Light light)
{
	float distance = length(light.position - worldPosition);
	float att = min(1.0 / (lights.attenuationConstants.x + (lights.attenuationConstants.y * distance) + (lights.attenuationConstants.z * (distance*distance))), 1.0);

	if(light.type == 0) // Directional
	{
		return ambient + diffuse + specular;
	}
	else if(light.type == 1)
	{
		return (ambient + diffuse + specular)*att;
	}
	else if(light.type == 2)
	{
		float spotEffect = 0;
		vec3 lightDir = normalize(light.direction);
		vec3 vertexToLightVector = normalize(worldPosition-light.position);
		float angle = dot(lightDir, vertexToLightVector);
		float phi = cos(radians(light.outerAngle));
		float theta = cos(radians(light.innerAngle));

		if(angle < phi)
			spotEffect = 0;
		else if(angle > theta)
			spotEffect = 1;
		else
			spotEffect = pow((angle-phi)/(theta-phi), light.fallOut);

		return att * spotEffect * (ambient + diffuse + specular);
	}

	return vec3(0);
}