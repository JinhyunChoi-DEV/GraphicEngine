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

layout (binding=0) uniform sampler2D positionBuffer;
layout (binding=1) uniform sampler2D normalBuffer;
layout (binding=2) uniform sampler2D uvBuffer;
layout (binding=3) uniform sampler2D depthBuffer;
layout (binding=4) uniform sampler2D ambientBuffer;
layout (binding=5) uniform sampler2D diffuseBuffer;
layout (binding=6) uniform sampler2D specularBuffer;
layout (binding=7) uniform sampler2D affectedLightingBuffer;

uniform vec3 cameraPosition;

in vec2 outputUV;

out vec4 outputColor;

float constStrenght = 0.5;
vec3 getLightDirection(in vec3 position, in Light light);
vec3 getAmbient(in Light light);
vec3 getDiffuse(in vec3 position, in vec3 normal, in Light light);
vec3 getSpecular(in vec3 position, in vec3 normal, in Light light);
vec3 getColorWithLight(in vec3 position, in vec3 ambient, in vec3 diffuse, in vec3 specular, in Light light);

void main()
{
	vec3 fragPosition = texture(positionBuffer, outputUV).rgb;
	vec3 fragNormal = normalize(texture(normalBuffer, outputUV).rgb);
	vec3 fragUV = texture(uvBuffer, outputUV).rgb;
	vec3 fragAmbient = texture(ambientBuffer, outputUV).rgb;
	vec3 fragDiffuse = texture(diffuseBuffer, outputUV).rgb;
	vec3 fragSpecular = texture(specularBuffer, outputUV).rgb;
	float depth = texture(depthBuffer, outputUV).r;
	bool affectedLighting = texture(affectedLightingBuffer, outputUV).r > 0.99;

	float S = (lights.far - length(cameraPosition - fragPosition))/(lights.far - lights.near);

	vec3 lightsResult = vec3(0);
	for(int i =0; i<lights.lightCount; ++i)
	{
		vec3 ambient = lights.light[i].ambient * fragAmbient;
		vec3 diffuse = getDiffuse(fragPosition, fragNormal, lights.light[i]) * fragDiffuse;
		vec3 specular = getSpecular(fragPosition, fragNormal, lights.light[i]) * fragSpecular;
		vec3 colorWithLight = getColorWithLight(fragPosition, ambient, diffuse, specular, lights.light[i]);

		lightsResult += max(colorWithLight, 0);
	}

	vec3 I_Local = lights.globalAmbientColor * constStrenght + lightsResult;
	vec3 colorResult = S*I_Local + (1.0-S)*lights.fogColor;

	if(!affectedLighting)
		outputColor = vec4(fragAmbient, 1.0);
	else
		outputColor = vec4(colorResult, 1.0);
}

vec3 getLightDirection(in vec3 position, in Light light)
{
	if(light.type == 0)
		return normalize(-light.direction);
	else
		return normalize(light.position - position);
}

vec3 getAmbient(in Light light)
{
	return light.ambient * constStrenght;
}

vec3 getDiffuse(in vec3 position, in vec3 normal, in Light light)
{
	vec3 lightDir = getLightDirection(position, light);
	float diff = max(dot(normal, lightDir), 0);

	return light.diffuse * diff;
}

vec3 getSpecular(in vec3 position, in vec3 normal, in Light light)
{
	vec3 lightDir = normalize(getLightDirection(position, light));

	vec3 viewVector = normalize(cameraPosition - position);
	vec3 reflection = 2.0 * dot(normal, lightDir) * normal - lightDir;
	float spec = pow(max(dot(viewVector, reflection), 0), 32.0);
	return light.specular * spec;
}

vec3 getColorWithLight(in vec3 position, in vec3 ambient, in vec3 diffuse, in vec3 specular, in Light light)
{
	float distance = length(light.position - position);
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
		vec3 vertexToLightVector = normalize(position-light.position);
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