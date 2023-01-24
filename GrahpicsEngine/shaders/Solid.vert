layout(std140, binding = 0) uniform Transform
{
	mat4 view;
	mat4 projection;
} transform;

uniform mat4 model;

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uvCoordinate;

void main()
{
	gl_Position = transform.projection * transform.view * model * vec4(position, 1.0f);
}