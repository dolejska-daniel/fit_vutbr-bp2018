///
/// @file ShaderManager.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <iostream>
#include <vector>
#include <Vars/Vars.h>
#include <geGL/geGL.h>

#include "Application.h"
#include "ShaderManager.h"
#include "ShaderLoader.h"

using namespace Application;
using namespace ge::gl;


ShaderManager::ShaderManager(vars::Vars& vars)
	: vars(vars)
{
}

ShaderManager::~ShaderManager()
{
}


void Application::ShaderManager::Use(const std::string programName)
{
	auto existingProgram = programs.find(programName);
	if (existingProgram != programs.end())
		return BindProgram(existingProgram->second);

	std::shared_ptr<Shader> shader;
	std::vector<std::shared_ptr<Shader>> shaders;

	ShaderSources sources = ShaderLoader::GetShaderSources(vars, programName);
	if (sources.Vertex.length())
	{
		shader = std::make_shared<Shader>();
		shader->create(GL_VERTEX_SHADER);
		shader->compile(sources.Vertex);

		shaders.push_back(shader);
	}
	if (sources.Geometry.length())
	{
		shader = std::make_shared<Shader>();
		shader->create(GL_GEOMETRY_SHADER);
		shader->compile(sources.Geometry);

		shaders.push_back(shader);
	}
	if (sources.Fragment.length())
	{
		shader = std::make_shared<Shader>();
		shader->create(GL_FRAGMENT_SHADER);
		shader->compile(sources.Fragment);

		shaders.push_back(shader);
	}

	std::shared_ptr<Program> program = std::make_shared<Program>(shaders);
	program->create();

	programs[programName] = program;
	BindProgram(program);
}

void Application::ShaderManager::BindProgram(std::shared_ptr<Program> program)
{
	activeProgram = program;
	program->use();
}
