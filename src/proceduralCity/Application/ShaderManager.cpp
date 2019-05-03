///
/// @file ShaderManager.cpp
/// @brief Obsahuje definice funkcí třídy Application::ShaderManager.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <vector>
#include <Vars/Vars.h>
#include <geGL/geGL.h>
#include <Application/Application.h>
#include <Application/ShaderManager.h>
#include <Application/ShaderLoader.h>
#include <BasicCamera/PerspectiveCamera.h>
#include <BasicCamera/FreeLookCamera.h>


using namespace Application;
using namespace ge::gl;

ShaderManager::ShaderManager(vars::Vars& vars)
	: _vars(vars)
{
}

ShaderManager::~ShaderManager()
= default;


void ShaderManager::Use(std::string const& programName)
{
	const auto existingProgram = _programs.find(programName);
	if (existingProgram != _programs.end())
		return BindProgram(existingProgram->second, programName);

	std::shared_ptr<Shader> shader;
	std::vector<std::shared_ptr<Shader>> shaders;

	auto sources = ShaderLoader::GetShaderSources(_vars, programName);
	if (sources.vertex.length())
	{
		shader = std::make_shared<Shader>();
		shader->create(GL_VERTEX_SHADER);
		shader->compile(sources.vertex);

		shaders.push_back(shader);
	}
	if (sources.geometry.length())
	{
		shader = std::make_shared<Shader>();
		shader->create(GL_GEOMETRY_SHADER);
		shader->compile(sources.geometry);

		shaders.push_back(shader);
	}
	if (sources.fragment.length())
	{
		shader = std::make_shared<Shader>();
		shader->create(GL_FRAGMENT_SHADER);
		shader->compile(sources.fragment);

		shaders.push_back(shader);
	}

	std::shared_ptr<Program> program = std::make_shared<Program>(shaders);
	program->create();

	_programs[programName] = program;
	BindProgram(program, programName);
}

void ShaderManager::BindProgram(std::shared_ptr<Program> const& program, std::string const& programName)
{
	_activeProgram = program;
	program->use();

	auto camera = Vars.get<basicCamera::FreeLookCamera>("camera");
	auto projection = Vars.get<basicCamera::PerspectiveCamera>("projection");

	program
		->setMatrix4fv("projectionMatrix", &projection->getProjection()[0][0])
		->setMatrix4fv("viewMatrix", &camera->getView()[0][0])
		->setMatrix4fv("modelMatrix", Vars.getReinterpret<float>("model"));

	if (programName == "Phong" || programName == "Phong_Terrain" || programName == "Phong_Buildings")
	{
		program
			->set3fv("lightPosition_worldspace", &camera->getPosition()[0])
			->set3fv("cameraPosition_worldspace", &camera->getPosition()[0]);
	}
}
