/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: LightStage.vert
Purpose: Making of vertex shader for calculate Light stage data
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
layout(location=1) in vec2 uvCoordinate;

out vec2 outputUV;

void main()
{
	outputUV = uvCoordinate;
	
	gl_Position = model * vec4(position, 1.0f);
}