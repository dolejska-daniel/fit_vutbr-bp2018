// ==========================================================dd=
//	VERTEX SHADER
// =============================================================
#shader vertex
#version 450

//	Input vertex attributes
layout(location = 0) in vec3 vertexPosition_worldspace;
layout(location = 1) in vec3 vertexNormal_worldspace;

//	Output variables
out vec3 fragmentColor;
out vec3 position;
out vec3 normal;

//	Uniforms
uniform vec3 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition_worldspace, 1);

	fragmentColor = color;

	position = vertexPosition_worldspace;
	normal = vertexNormal_worldspace;
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
in vec3 fragmentColor;
in vec3 position;
in vec3 normal;

//	Output variables
layout(location = 0) out vec4 color;

//	Uniforms
uniform vec3 cameraPosition_worldspace;
uniform vec3 lightPosition_worldspace;

uniform vec3 lightColor = vec3(1, 1, 1);


void main()
{
	if (normal == 0)
	{
		color = vec4(fragmentColor, 1);
		return;
	}

	float pxFactor = clamp(dot(normal, vec3(+1, 0, 0)), 0, 1);
	float nxFactor = clamp(dot(normal, vec3(-1, 0, 0)), 0, 1);
	float pyFactor = clamp(dot(normal, vec3(0, +1, 0)), 0, 1);
	float nyFactor = clamp(dot(normal, vec3(0, -1, 0)), 0, 1);
	float pzFactor = clamp(dot(normal, vec3(0, 0, +1)), 0, 1);
	float nzFactor = clamp(dot(normal, vec3(0, 0, -1)), 0, 1);

	vec2 pxCoord = position.zy * vec2(-1, +1) * .5 + .5;
	vec2 nxCoord = position.zy * vec2(+1, +1) * .5 + .5;
	vec2 pyCoord = position.xz * vec2(+1, -1) * .5 + .5;
	vec2 nyCoord = position.xz * vec2(+1, +1) * .5 + .5;
	vec2 pzCoord = position.xy * vec2(+1, +1) * .5 + .5;
	vec2 nzCoord = position.xy * vec2(-1, +1) * .5 + .5;

	vec3 pxDiffuseColor = texture(diffuseTextureDirt, pxCoord).xyz * pxFactor;
	vec3 nxDiffuseColor = texture(diffuseTextureDirt, nxCoord).xyz * nxFactor;
	vec3 pyDiffuseColor = texture(diffuseTextureGrass,  pyCoord).xyz * pyFactor;
	vec3 nyDiffuseColor = texture(diffuseTextureRock, nyCoord).xyz * nyFactor;
	vec3 pzDiffuseColor = texture(diffuseTextureDirt, pzCoord).xyz * pzFactor;
	vec3 nzDiffuseColor = texture(diffuseTextureDirt, nzCoord).xyz * nyFactor;

	vec3 diffColor =
		pxDiffuseColor +
		nxDiffuseColor +
		pyDiffuseColor +
		nyDiffuseColor +
		pzDiffuseColor +
		nzDiffuseColor;


	vec3 phong = 0.3 * diffColor + 0.6 * diffColor + 0.0 * lightColor;

	color = vec4(phong, 1);
	return;

	/*
	vec3 light_pos = lightPosition_worldspace;
	vec3 camera_pos = cameraPosition_worldspace;

	vec3 light_color = vec3(1, 1, 1);
	vec3 light_offset = light_pos - position;
	vec3 light_direction = normalize(light_offset);
	vec3 norm_vec = normalize(normal);

	// diffuse
	float diff = max(dot(norm_vec, light_direction), 0);
	vec3 diffuse = fragmentColor * diff;

	// specular
	vec3 light_direction_offset = light_direction * -1;
	vec3 reflection_direction = reflect(light_direction_offset, norm_vec);
	vec3 camera_offset = camera_pos - position;
	vec3 view_direction = normalize(camera_offset);

	float spec = pow(max(dot(view_direction, reflection_direction), 0), 60);
	vec3 specular = light_color * spec;

	color = diffuse + specular;
	*/
}

