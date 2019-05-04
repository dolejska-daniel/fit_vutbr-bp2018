// ==========================================================dd=
//	VERTEX SHADER
//	https://learnopengl.com/Advanced-OpenGL/Geometry-Shader
// =============================================================
#shader vertex
#version 450

//	Input vertex attributes
layout(location = 0) in vec3 vertexPosition_worldspace;
layout(location = 1) in vec3 vertexNormal_worldspace;

//	Output variables
out vec3 position;
out vec3 normal;

out VS_OUT {
	vec3 position;
	vec3 normal;
} vs_out;

//	Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition_worldspace, 1);

	position = vertexPosition_worldspace;
	normal = normalize(vertexNormal_worldspace);
}


// ==========================================================dd=
//	GEOMETRY SHADER
//	Pøevzato a upraveno z: https://github.com/martin-pr/possumwood/wiki/Wireframe-using-a-Geometry-Shader
// =============================================================
#shader geometry
#version 450

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

// Input variables
in vec3 position[];
in vec3 normal[];

// Output variables
out vec3 fragmentColor;
out vec3 vertexPosition;
out vec3 vertexNormal;

uniform vec3 color = vec3(0, 1, 0);

void main(void)
{
	fragmentColor = color;
	for (int i = 0; i < 4; i++)
	{
		gl_Position = gl_in[i % 3].gl_Position;
		vertexPosition = position[i % 3];
		vertexNormal = normal[i % 3];
		EmitVertex();
	}
	EndPrimitive();
}


// ==========================================================dd=
//	FRAGMENT SHADER
// =============================================================
#shader fragment
#version 450

//	Input variables
in vec3 fragmentColor;
in vec3 vertexPosition;
in vec3 vertexNormal;

//	Output variables
layout(location = 0) out vec4 color;

//	Uniforms
uniform vec3 cameraPosition_worldspace;
uniform vec3 lightPosition_worldspace;

uniform vec3 lightColor = vec3(1, 1, 1);

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

void main(void)
{
	color = vec4(phongLighting(
		vertexPosition,
		vertexNormal,
		lightPosition_worldspace,
		cameraPosition_worldspace,
		1000,
		fragmentColor,
		lightColor
	), 1.0);
}