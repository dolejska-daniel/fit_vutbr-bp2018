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
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition_worldspace, 1);

	position = vertexPosition_worldspace;
	normal = vertexNormal_worldspace;
}


// ==========================================================dd=
//	FRAGMENT SHADER
// =============================================================
#shader fragment
#version 450

//	Input variables
in vec3 position;
in vec3 normal;

//	Output variables
layout(location = 0) out vec3 color;


void main()
{
	color = normal * .5 + .5;
}

