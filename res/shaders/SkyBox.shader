// ==========================================================dd=
//	VERTEX SHADER
// =============================================================
#shader vertex
#version 450

//	Input vertex attributes
layout(location = 0) in vec3 vertexPosition_worldspace;

//	Output variables
out vec3 textureCoords;

//	Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition_worldspace, 1);
	textureCoords = vertexPosition_worldspace;
}


// ==========================================================dd=
//	FRAGMENT SHADER
// =============================================================
#shader fragment
#version 450

//	Input textures
uniform samplerCube skyboxTexture;

//	Input variables
in vec3 textureCoords;

//	Output variables
layout(location = 0) out vec4 color;

void main()
{
	color = texture(skyboxTexture, textureCoords);
}

