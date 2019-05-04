// ==========================================================dd=
//	VERTEX SHADER
// =============================================================
#shader vertex
#version 450

//	Input vertex attributes
layout(location = 0) in vec3 vertexPosition_worldspace;
layout(location = 1) in vec3 vertexNormal_worldspace;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in int textureSlot;

//	Output variables
out vec3 position;
out vec3 normal;
out vec2 v_textureCoord;
out int v_textureSlot;

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
	v_textureSlot = textureSlot;
}


// ==========================================================dd=
//	FRAGMENT SHADER
// =============================================================
#shader fragment
#version 450

//	Input textures
layout(binding = 3) uniform sampler2D diffuseTextureConcreteBlock;
layout(binding = 4) uniform sampler2D diffuseTextureConcrete2;
layout(binding = 5) uniform sampler2D diffuseTextureConcrete3;
layout(binding = 6) uniform sampler2D specularTextureWindows;

//	Input variables
in vec3 position;
in vec3 normal;
in vec2 v_textureCoord;
flat in int v_textureSlot;

//	Output variables
layout(location = 0) out vec4 color;

//	Uniforms
uniform vec3 cameraPosition_worldspace;
uniform vec3 lightPosition_worldspace;

uniform vec3  lightColor   = vec3(1, 1, 1);
uniform float textureScale = 1;
uniform float shininess = 1000;

uniform samplerCube skyboxTexture;

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

	float pxFactor = clamp(dot(normal, vec3(+1, 0, 0)), 0, 1);
	float nxFactor = clamp(dot(normal, vec3(-1, 0, 0)), 0, 1);
	float pyFactor = clamp(dot(normal, vec3(0, +1, 0)), 0, 1);
	float nyFactor = clamp(dot(normal, vec3(0, -1, 0)), 0, 1);
	float pzFactor = clamp(dot(normal, vec3(0, 0, +1)), 0, 1);
	float nzFactor = clamp(dot(normal, vec3(0, 0, -1)), 0, 1);

	float scale = 1 / textureScale;
	vec2 pxCoord = position.zy * vec2(-1, +1) * scale;
	vec2 nxCoord = position.zy * vec2(+1, +1) * scale;
	vec2 pyCoord = position.xz * vec2(+1, -1) * scale;
	vec2 nyCoord = position.xz * vec2(+1, +1) * scale;
	vec2 pzCoord = position.xy * vec2(+1, +1) * scale;
	vec2 nzCoord = position.xy * vec2(-1, +1) * scale;

	vec3 pxDiffuseColor = texture(diffuseTextureConcreteBlock, pxCoord).xyz * pxFactor;
	vec3 nxDiffuseColor = texture(diffuseTextureConcreteBlock, nxCoord).xyz * nxFactor;

	vec3 pyDiffuseColor = texture(diffuseTextureConcrete3, pyCoord).xyz * pyFactor;
	vec3 nyDiffuseColor = texture(diffuseTextureConcrete3, nyCoord).xyz * nyFactor;

	vec3 pzDiffuseColor = texture(diffuseTextureConcreteBlock, pzCoord).xyz * pzFactor;
	vec3 nzDiffuseColor = texture(diffuseTextureConcreteBlock, nzCoord).xyz * nyFactor;

	vec3 diffColor = vec3(0, 0, 0);
	diffColor = diffColor +
		pxDiffuseColor +
		nxDiffuseColor +
		pyDiffuseColor +
		nyDiffuseColor +
		pzDiffuseColor +
		nzDiffuseColor;

	vec3 windowSpecular = texture(specularTextureWindows, v_textureCoord).rgb;
	if (greaterThan(windowSpecular, vec3(0, 0, 0)) == true)
	{
		vec3 I = normalize(position - cameraPosition_worldspace);
		vec3 R = reflect(I, normal);
		diffColor = texture(skyboxTexture, R).rgb;
	}
	/*
	vec3 diffColor;
	if (v_textureSlot == 0)
	{
		diffColor = vec3(1, 0, 0);
	}
	else if (v_textureSlot  == 1)
	{
		diffColor = vec3(0, 1, 0);
	}
	else if (v_textureSlot == 2)
	{
		diffColor = vec3(0, 0, 1);
	}
	else
	{
		diffColor = texture(specularTextureWindows, v_textureCoord).xyz;
	}*/

	vec3 phong = ambientFactor * diffColor + diffuseFactor * diffColor + specularFactor * lightColor;
	color = vec4(phong, 1);
}

