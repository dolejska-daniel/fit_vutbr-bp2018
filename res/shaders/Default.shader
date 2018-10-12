#shader vertex
#version 450

//	Input vertex attributes
layout(location = 0) in vec3 vertexPosition_worldspace;
layout(location = 1) in vec3 vertexNormal_worldspace;
//layout(location = 2) in vec3 vertexColor;

//	Output variables
out vec3 fragmentColor;
out vec3 position;
out vec3 normal;

//	Uniforms
uniform mat4 mvpMatrix;


void main()
{
	gl_Position = mvpMatrix * vec4(vertexPosition_worldspace, 1);

	fragmentColor = vec3(0, 1, 0);

	position = vertexPosition_worldspace;
	normal = vertexNormal_worldspace;
}



#shader fragment
#version 450

//	Input variables
in vec3 fragmentColor;
in vec3 position;
in vec3 normal;

//	Output variables
layout(location = 0) out vec3 color;

//	Uniforms
uniform vec3 cameraPosition_worldspace;
uniform vec3 lightPosition_worldspace;


void main()
{
	//	Use color provided by vertex shader
	//color = fragmentColor;

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
}

