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

	auto projectionMatrix = projection->getProjection();
	auto viewMatrix = camera->getView();
	auto cameraPosition = camera->getPosition();

	if (programName == "SkyBox")
	{
		viewMatrix = glm::mat4(glm::mat3(viewMatrix));
	}

	program
		->setMatrix4fv("projectionMatrix", &projectionMatrix[0][0])
		->setMatrix4fv("viewMatrix", &viewMatrix[0][0])
		->setMatrix4fv("modelMatrix", Vars.getReinterpret<float>("model"));

	if (programName == "Phong"
		|| programName == "Phong_Terrain"
		|| programName == "Phong_Buildings"
		|| programName == "Geometry_Wireframe"
		)
	{
		program
			->set3fv("lightPosition_worldspace", &cameraPosition[0])
			->set3fv("cameraPosition_worldspace", &cameraPosition[0]);
	}
}
