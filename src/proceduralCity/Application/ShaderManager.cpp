///
/// @file ShaderManager.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///

#include <iostream>

#include <geGL/StaticCalls.h>

#include "Application.h"
#include "ShaderManager.h"
#include "ShaderLoader.h"

using namespace Application;
using namespace ge::gl;


ShaderSources	ShaderManager::sources;	///< Contains ShaderSources struct
unsigned int	ShaderManager::program;	///< Program ID
unsigned int	ShaderManager::vs;		///< Vertex Shader ID
unsigned int	ShaderManager::fs;		///< Fragment Shader ID

///
/// @brief
///
/// @param
///
void ShaderManager::init(const std::string filename)
{
	ShaderLoader::setSourceFilename(filename);
	sources = ShaderLoader::parse();

	program = glCreateProgram();

	std::cerr << "sources.Vertex.length(): " << sources.Vertex.length() << std::endl;
	if (sources.Vertex.length())
		compile(GL_VERTEX_SHADER, sources.Vertex, &vs);

	std::cerr << "sources.Fragment.length(): " << sources.Fragment.length() << std::endl;
	if (sources.Fragment.length())
		compile(GL_FRAGMENT_SHADER, sources.Fragment, &fs);
}

///
/// @brief
///
void ShaderManager::attach()
{
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	int program_linked;
	glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE)
	{
		int log_length = 0;
		char message[1024];
		glGetProgramInfoLog(program, 1024, &log_length, message);
		std::cerr << "Failed to link program: " << message << std::endl;
	}

	int program_valid;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &program_valid);
	if (program_valid != GL_TRUE)
	{
		int log_length = 0;
		char message[1024];
		glGetProgramInfoLog(program, 1024, &log_length, message);
		std::cerr << "Failed to validate program: " << message << std::endl;
	}

	glUseProgram(program);
}

///
/// @brief
///
void ShaderManager::detach()
{
	glDetachShader(program, vs);
	glDeleteShader(vs);

	glDetachShader(program, fs);
	glDeleteShader(fs);

	glDeleteProgram(program);
}

///
/// @brief
///
void ShaderManager::compile(GLenum type, std::string source, unsigned int* shader_id)
{
	*shader_id = glCreateShader(type);
	int shader = *shader_id;

	std::cerr << "created shader: " << shader << std::endl;
	std::cerr << source.c_str() << std::endl;

	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	int shader_compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compiled);
	if (shader_compiled != GL_TRUE)
	{
		int log_length = 0;
		char message[1024];
		glGetShaderInfoLog(shader, 1024, &log_length, message);
		std::cerr << "Failed to compile shader: " << message << std::endl;
	}
}