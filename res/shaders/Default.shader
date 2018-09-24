#shader vertex
#version 450

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
};



#shader fragment
#version 450

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
};
