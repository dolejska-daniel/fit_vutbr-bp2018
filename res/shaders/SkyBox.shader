// ==========================================================dd=
//	VERTEX SHADER
//	pøevzato z: https://learnopengl.com/Advanced-OpenGL/Cubemaps
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
//	pøevzato z: https://learnopengl.com/Advanced-OpenGL/Cubemaps
// =============================================================
#shader fragment
#version 450

layout(binding = 7) uniform samplerCube skyboxTexture;
layout(binding = 8) uniform samplerCube cityboxTexture;

//	Input variables
in vec3 textureCoords;

//	Output variables
layout(location = 0) out vec4 color;

void main()
{
	vec3 skyboxColor = texture(skyboxTexture, textureCoords).rgb;
	color = vec4(skyboxColor, 1);
}

