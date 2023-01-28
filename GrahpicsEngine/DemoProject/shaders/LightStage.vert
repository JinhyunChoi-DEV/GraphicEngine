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