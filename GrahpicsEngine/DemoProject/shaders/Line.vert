layout(std140, binding = 0) uniform Transform
{
	mat4 view;
	mat4 projection;
} transform;

uniform mat4 model;

layout(location=0) in vec3 position;

void main()
{
	gl_Position = transform.projection * transform.view * model * vec4(position, 1.0f);
}