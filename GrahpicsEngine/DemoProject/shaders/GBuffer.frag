/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: GBuffer.frag
Purpose: Making of fragment shader for GBuffer data
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

layout(location=0) out vec3 gBufferPosition;
layout(location=1) out vec3 gBufferNormal;
layout(location=2) out vec3 gBufferUV;
layout(location=3) out vec3 gBufferDepth;
layout(location=4) out vec3 gBufferAmbient;
layout(location=5) out vec3 gBufferDiffuse;
layout(location=6) out vec3 gBufferSpecular;
layout(location=7) out vec3 gBufferAffectedLighting;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform bool useAmbientTexture;
uniform bool useDiffuseTexture;
uniform bool useSpecularTexture;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform bool useGpuUV;
uniform bool affectedLighting;

in vec3 modelPosition;
in vec3 worldPosition;
in vec3 outputNormal;
in vec3 outputUV;

vec3 calculateSphereUV(in vec3 position);

void main()
{
	vec3 uv;
	if(useGpuUV)
		uv = calculateSphereUV(modelPosition);
	else
		uv = outputUV;

	gBufferPosition = worldPosition;
	gBufferNormal = normalize(outputNormal);
	gBufferUV = uv;
	gBufferAffectedLighting = vec3(affectedLighting, 0, 0);

	float distance = length(worldPosition);
	gBufferDepth = vec3(distance);

	if(useAmbientTexture)
		gBufferAmbient = texture(ambientTexture, uv.st).rgb * ambientColor;
	else
		gBufferAmbient = ambientColor;

	if(useDiffuseTexture)
		gBufferDiffuse = texture(diffuseTexture, uv.st).rgb * diffuseColor;
	else
		gBufferDiffuse = diffuseColor;

	if(useSpecularTexture)
		gBufferSpecular = texture(specularTexture, uv.st).rgb * specularColor;
	else
		gBufferSpecular = specularColor;
}

vec3 calculateSphereUV(in vec3 position)
{
	vec3 uv = vec3(0,0,1);

	float x = position.x;
	float y = position.y;
	float z = position.z;
	float r = sqrt(x*x + y*y + z*z);
	float theta = degrees(atan(position.z, position.x)) + 180.0f;
	float phi = degrees(acos(y/r));

	uv.x = theta/360.f;
	uv.y = 1.0-(phi/180.0f);
	return uv;
}