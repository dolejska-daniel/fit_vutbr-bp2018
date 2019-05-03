// ==========================================================dd=
//	VERTEX SHADER
// =============================================================
#shader vertex
#version 450

//	Input vertex attributes
layout(location = 0) in vec3 vertexPosition_worldspace;
layout(location = 1) in vec3 vertexNormal_worldspace;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in uint textureSlot;

//	Output variables
out vec3 position;
out vec3 normal;
out vec2 v_textureCoord;

//	Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition_worldspace, 1);

	position = vertexPosition_worldspace;
	normal = normalize(vertexNormal_worldspace);

	v_textureCoord = textureCoord;
}


// ==========================================================dd=
//	FRAGMENT SHADER
// =============================================================
#shader fragment
#version 450

//	Input textures
layout(binding = 3) uniform sampler2D specularTextureWindows;

//	Input variables
in vec3 position;
in vec3 normal;
in vec2 v_textureCoord;

//	Output variables
layout(location = 0) out vec4 color;

//	Uniforms
uniform vec3 cameraPosition_worldspace;
uniform vec3 lightPosition_worldspace;

uniform vec3  lightColor   = vec3(1, 1, 1);
uniform float textureScale = 1;
uniform float shininess = 1000;

// ----------------------------------------------------dd--
//	LIGHTING FUNCTIONS
// ----------------------------------------------------dd--

float diffuse(
	vec3 position,
	vec3 normal,
	vec3 light)
{
	vec3 l = normalize(light - position);

	return clamp(dot(normal, l), 0, 1);
}

float specular(
	vec3 position,
	vec3 normal,
	vec3 light,
	vec3 camera,
	float shininess)
{
	vec3 l = normalize(light - position);
	vec3 v = normalize(camera - position);
	vec3 r = reflect(v, normal);

	return pow(clamp(dot(l, -r), 0, 1), shininess)
		* sign(diffuse(position, normal, light));
}

vec3 lambertLighting(
	vec3 position,
	vec3 normal,
	vec3 light,
	vec3 diffuseColor)
{
	float ambientFactor = 0.4;
	float diffuseFactor = diffuse(position, normal, light);

	return ambientFactor * diffuseColor +
		diffuseFactor * diffuseColor;
}

vec3 phongLighting(
	vec3 position,
	vec3 normal,
	vec3 light,
	vec3 camera,
	float shininess,
	vec3 diffuseColor,
	vec3 lightColor)
{
	float ambientFactor = 0.4;
	float diffuseFactor = diffuse(position, normal, light);
	float specularFactor = specular(position, normal, light, camera, shininess);

	return ambientFactor * diffuseColor +
		diffuseFactor * diffuseColor +
		specularFactor * lightColor;
}


// ----------------------------------------------------dd--
//	MAIN
// ----------------------------------------------------dd--

void main()
{
	float ambientFactor = 0.3;
	float diffuseFactor = diffuse(position, normal, lightPosition_worldspace);
	float specularFactor = specular(position, normal, lightPosition_worldspace, cameraPosition_worldspace, shininess);

	vec3 diffColor = texture(specularTextureWindows, v_textureCoord).xyz;

	vec3 phong = ambientFactor * diffColor + diffuseFactor * diffColor + specularFactor * lightColor;
	color = vec4(phong, 1);
}

