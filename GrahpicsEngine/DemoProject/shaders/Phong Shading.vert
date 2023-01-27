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
out vec2 outputUV;

void main()
{
	modelPosition = position;
	worldPosition = vec3(model * vec4(position, 1.0f));
	outputNormal = mat3(transpose(inverse(model))) * normal;
	outputUV = uvCoordinate;

	gl_Position = transform.projection * transform.view * vec4(worldPosition, 1.0f);
}