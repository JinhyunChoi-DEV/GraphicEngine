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


in vec3 Test;
out vec4 FragColor;

void main()
{
	FragColor = vec4(Test,1.0);
}