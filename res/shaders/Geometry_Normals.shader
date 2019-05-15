// ==========================================================dd=
//	VERTEX SHADER
//	Pøevzato a upraveno z: https://learnopengl.com/Advanced-OpenGL/Geometry-Shader
// =============================================================
#shader vertex
#version 450

//	Input vertex attributes
layout(location = 0) in vec3 vertexPosition_worldspace;
layout(location = 1) in vec3 vertexNormal_worldspace;

//	Output variables
out vec3 normal;
out VS_OUT {
	vec3 normal;
} vs_out;

//	Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition_worldspace, 1);

	mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
	vs_out.normal = normalize(vec3(projectionMatrix * vec4(normalMatrix * vertexNormal_worldspace, 0.0)));
}


// ==========================================================dd=
//	GEOMETRY SHADER
//	Pøevzato a upraveno z:  https://learnopengl.com/Advanced-OpenGL/Geometry-Shader
// =============================================================
#shader geometry
#version 450

layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

out vec3 fragmentColor;

in VS_OUT {
	vec3 normal;
} gs_in[];

const float MAGNITUDE = 6;

void GenerateNormal(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
	EmitVertex();

	EndPrimitive();
}

void main(void)
{
	fragmentColor = vec3(1, 0, 0);
	GenerateNormal(0); // first vertex normal
	GenerateNormal(1); // second vertex normal
	GenerateNormal(2); // third vertex normal
}


// ==========================================================dd=
//	FRAGMENT SHADER
// =============================================================
#shader fragment
#version 450

in vec3 fragmentColor;

//	Output variables
layout(location = 0) out vec4 color;


void main(void)
{
	color = vec4(fragmentColor, 1.0);
}