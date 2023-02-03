/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: GBuffer.vert
Purpose: Making of vertex shader for GBuffer data
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

layout(std140, binding = 0) uniform Transform
{
	mat4 view;
	mat4 projection;
} transform;

uniform mat4 model;

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uvCoordinate;

out vec3 modelPosition;
out vec3 worldPosition;
out vec3 outputNormal;
out vec3 outputUV;

void main()
{
	modelPosition = position;
	worldPosition = vec3(model * vec4(position, 1.0f));
	outputNormal = vec3(model * vec4(normal, 0.0f));
	outputUV = vec3(uvCoordinate.st, 1.0);

	gl_Position = transform.projection * transform.view * model * vec4(position, 1.0f);
}