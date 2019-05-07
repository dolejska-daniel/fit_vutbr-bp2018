// ==========================================================dd=
//	VERTEX SHADER
// =============================================================
#shader vertex
#version 450

//	Input vertex attributes
layout(location = 0) in vec3 vertexPosition_worldspace;
layout(location = 1) in vec3 vertexNormal_worldspace;
layout(location = 2) in vec3 textureMix;

//	Output variables
out vec3 position;
out vec3 normal;
out vec3 v_textureMix;

//	Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition_worldspace, 1);

	position = vertexPosition_worldspace;
	normal = normalize(vertexNormal_worldspace);
	v_textureMix = textureMix;
}


// ==========================================================dd=
//	FRAGMENT SHADER
// =============================================================
#shader fragment
#version 450

//	Input textures
layout(binding = 0) uniform sampler2D diffuseTextureGrass;
layout(binding = 1) uniform sampler2D diffuseTextureDirt;
layout(binding = 2) uniform sampler2D diffuseTextureRock;

//	Input variables
in vec3 position;
in vec3 normal;
in vec3 v_textureMix;

//	Output variables
layout(location = 0) out vec4 color;

//	Uniforms
uniform vec3 cameraPosition_worldspace;
uniform vec3 lightPosition_worldspace;

uniform vec3  lightColor   = vec3(1, 1, 1);
uniform float textureScale = 50;
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


	vec3 diffColor =
		texture(diffuseTextureDirt, pxCoord).xyz * pxFactor * v_textureMix.x +
		texture(diffuseTextureDirt, nxCoord).xyz * nxFactor * v_textureMix.x +
		texture(diffuseTextureDirt, pyCoord).xyz * pyFactor * v_textureMix.x +
		texture(diffuseTextureDirt, nyCoord).xyz * nyFactor * v_textureMix.x +
		texture(diffuseTextureDirt, pzCoord).xyz * pzFactor * v_textureMix.x +
		texture(diffuseTextureDirt, nzCoord).xyz * nzFactor * v_textureMix.x +

		texture(diffuseTextureGrass, pxCoord).xyz * pxFactor * v_textureMix.y +
		texture(diffuseTextureGrass, nxCoord).xyz * nxFactor * v_textureMix.y +
		texture(diffuseTextureGrass, pyCoord).xyz * pyFactor * v_textureMix.y +
		texture(diffuseTextureGrass, nyCoord).xyz * nyFactor * v_textureMix.y +
		texture(diffuseTextureGrass, pzCoord).xyz * pzFactor * v_textureMix.y +
		texture(diffuseTextureGrass, nzCoord).xyz * nzFactor * v_textureMix.y +

		texture(diffuseTextureRock, pxCoord).xyz * pxFactor * v_textureMix.z +
		texture(diffuseTextureRock, nxCoord).xyz * nxFactor * v_textureMix.z +
		texture(diffuseTextureRock, pyCoord).xyz * pyFactor * v_textureMix.z +
		texture(diffuseTextureRock, nyCoord).xyz * nyFactor * v_textureMix.z +
		texture(diffuseTextureRock, pzCoord).xyz * pzFactor * v_textureMix.z +
		texture(diffuseTextureRock, nzCoord).xyz * nzFactor * v_textureMix.z;

	diffColor /= 2;
	vec3 phong = ambientFactor * diffColor + diffuseFactor * diffColor + specularFactor * lightColor;
	color = vec4(phong, 1);
}

