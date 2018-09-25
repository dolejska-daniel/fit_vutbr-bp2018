#shader vertex
#version 450

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

uniform mat4 mvpMatrix;

out vec3 fragmentColor;

void main()
{
	gl_Position = mvpMatrix * vec4(vertexPosition_modelspace, 1);

	fragmentColor = vertexColor;
};



#shader fragment
#version 450

in vec3 fragmentColor;

layout(location = 0) out vec3 color;

void main()
{
	color = fragmentColor;
};
