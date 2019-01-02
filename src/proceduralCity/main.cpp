///
/// @file main.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2CPP/Window.h>
#include <SDL2CPP/MainLoop.h>
#include <glm/glm.hpp>
#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <BasicCamera/PerspectiveCamera.h>
#include <BasicCamera/FreeLookCamera.h>
#include <ArgumentViewer/ArgumentViewer.h>
#include <Vars/Vars.h>
#include <Terrain/Map.h>
#include <Terrain/Chunk.h>
#include <Terrain/Generator.h>
#include <Application/Application.h>
#include <Application/ShaderManager.h>
#include <Application/Renderer.h>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetMap.h>
#include <Infrastructure/StreetNode.h>


using namespace glm;
using namespace ge::gl;
using namespace vars;
using namespace argumentViewer;
using namespace Application;
using namespace Terrain;

///
/// @brief
///
/// @param argc	number of arguments
/// @param argv	array of arguments
///
int main(const int argc, char* argv[])
{
	ArgumentViewer args(argc, argv);
	Vars vars;

	// ==========================================================dd=
	//	DEFINICE OBECNÝCH PROMĚNNÝCH A ARGUMENTŮ PROGRAMU
	// =============================================================
	vars.addString("resources.dir", args.gets("--resources-dir", "../res", "Path to resources directory (shaders, ...)"));

	// ==========================================================dd=
	//	DEFINICE PROMĚNNÝCH A ARGUMENTŮ PROGRAMU PRO NASTAVENÍ TERÉNU
	// =============================================================
	vars.addUint32("terrain.seed",			args.getu32("--terrain-seed",			12345,	"Default seed for terrain generation"));
	vars.addFloat( "terrain.scale",			args.getf32("--terrain-scale",			32.f,	""));
	vars.addFloat( "terrain.amplitude",		args.getf32("--terrain-amplitude",		1.5f,	""));
	vars.addFloat( "terrain.frequency",		args.getf32("--terrain-frequency",		2.f,	""));
	vars.addFloat( "terrain.persistence",	args.getf32("--terrain-persistence",	0.5f,	""));
	vars.addFloat( "terrain.lacunarity",	args.getf32("--terrain-lacunarity",		1.25f,	""));
	vars.addUint32("terrain.octaves",		args.getu32("--terrain-octaves",		3,		"Number of noises from which is the terrain going to be generated"));
	vars.addUint32("terrain.detail",		args.getu32("--terrain-detail",			1,		"Default level of object detail"));

	vars.addUint32("terrain.map.width",		args.getu32("--terrain-map-width",	1, "Terrain map width (count of chunks)"));
	vars.addUint32("terrain.map.height",	args.getu32("--terrain-map-height", 1, "Terrain map height (count of chunks) in 2D (depth/length in 3D)"));

	vars.addUint32("terrain.chunk.width",	args.getu32("--terrain-chunk-width",	64,		"Chunk width"));
	vars.addUint32("terrain.chunk.height",	args.getu32("--terrain-chunk-height",	64 ,		"Terrain chunk height in 2D (depth/length in 3D)"));
	vars.addFloat( "terrain.chunk.scale",	args.getf32("--terrain-chunk-scale",	1.f,	"Chunk scale multiplier"));

	if (args.isPresent("--help", "") || args.validate() == false)
	{
		std::cerr << args.toStr() << std::endl;
		exit(EXIT_SUCCESS);
	}


	// ==========================================================dd=
	//	INICIALIZACE OKNA, geGL, ...
	// =============================================================
	uint32_t width = 1024;
	uint32_t height = 768;

	//	Create main loop & window
	auto mainLoop = std::make_shared<sdl2cpp::MainLoop>();
	auto window   = std::make_shared<sdl2cpp::Window  >(width, height);
    window->createContext("rendering", 450u, sdl2cpp::Window::CORE, sdl2cpp::Window::DEBUG);
    mainLoop->addWindow("mainWindow", window);

	//	Initialize geGL
	init();

	// Enable debug mode
	setHighDebugMessage();
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	std::cerr << "Using " << glGetString(GL_VERSION) << std::endl;


	// ==========================================================dd=
	//	DALŠÍ INICIALIZACE
	// =============================================================
	Infrastructure::StreetRootNode = std::make_shared<Infrastructure::StreetNode>(vec2(0.f), 8.f);
	Infrastructure::StreetRootNode->SetRoot();

	//	Initialize ShaderManager
	auto shaders = std::make_shared<ShaderManager>(vars);
	shaders->Use("Phong");

	auto renderer = std::make_shared<Renderer>(vars);
	auto color = vec3(0, 1, 0);

	//	Setup main camera
	auto cameraProjection = std::make_shared<basicCamera::PerspectiveCamera>(radians(45.f), width / height, 0.1f, INFINITY);
	auto freeLook = std::make_shared<basicCamera::FreeLookCamera>();
	freeLook->setPosition(vec3(1, 1, 4));

	const auto map = Generator::GenerateMap(vars);

	auto streetMap = Infrastructure::StreetMap(vars);


	// ==========================================================dd=
	//	VYKRESLOVÁNÍ
	// =============================================================

	std::cerr << "Callback" << std::endl;
	//	Drawing
    mainLoop->setIdleCallback([&](){

		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto a = 0; a < 3; a++)
			freeLook->move(a, float(KeyDown["d s"[a]] - KeyDown["acw"[a]]) * float(.25f + KeyDown[SDLK_LSHIFT] * 2.f));

	    auto cameraPosition = freeLook->getPosition();
	    auto projectionMatrix = cameraProjection->getProjection();
	    auto viewMatrix = freeLook->getView();
		mat4 modelMatrix(1);

		color = vec3(0, 1, 0);
		shaders->GetActiveProgram()->set3fv("color", &color[0]);

		shaders->GetActiveProgram()->set3fv("lightPosition_worldspace", &cameraPosition[0]);
		shaders->GetActiveProgram()->set3fv("cameraPosition_worldspace", &cameraPosition[0]);
		shaders->GetActiveProgram()->setMatrix4fv("projectionMatrix", &projectionMatrix[0][0]);
		shaders->GetActiveProgram()->setMatrix4fv("viewMatrix", &viewMatrix[0][0]);
		shaders->GetActiveProgram()->setMatrix4fv("modelMatrix", &modelMatrix[0][0]);

		for (unsigned int y = 0; y < map->GetHeight(); y++)
		{
			for (unsigned int x = 0; x < map->GetWidth(); x++)
			{
				const auto chunk = map->ReadChunk(x, y);
				renderer->Render(chunk);
			}
		}

		color = vec3(1, 1, 1);
		shaders->GetActiveProgram()->set3fv("color", &color[0]);

		if (KeyDown['x'])
		{
			streetMap.BuildStep();
		}

		static auto built = false;
		if (KeyDown['v'])
		{
			if (!built)
			{
				std::cerr << std::endl << "Building..." << std::endl;
				streetMap.BuildStep();
				built = true;
			}
		}
		else
			built = false;

		if (KeyDown['y'])
		{
			printf("Number of streets: %lld\n", streetMap.ReadStreets().size());
		}

		for (const auto& street : streetMap.ReadStreets())
		{
			renderer->Render(street);
		}

		glFinish();
        window->swap();
    });

	//	Camera rotation
	window->setEventCallback(SDL_MOUSEMOTION, [&](SDL_Event const& event) {
		if (event.motion.state & SDL_BUTTON_LMASK)
		{
			//	Trap mouse
			SDL_SetRelativeMouseMode(SDL_TRUE);
			
			auto const sensitivity = 0.0025f;
			auto const xrel = static_cast<float>(event.motion.xrel);
			auto const yrel = static_cast<float>(event.motion.yrel);

			freeLook->setAngle(1, freeLook->getAngle(1) + xrel * sensitivity);
			freeLook->setAngle(0, freeLook->getAngle(0) + yrel * sensitivity);

			return true;
		}

		//	Free mouse
		SDL_SetRelativeMouseMode(SDL_FALSE);
		return false;
	});

	window->setEventCallback(SDL_KEYDOWN, [&](SDL_Event const& event) {
		KeyDown[event.key.keysym.sym] = true;
		return true;
	});

	window->setEventCallback(SDL_KEYUP, [&](SDL_Event const& event) {
		KeyDown[event.key.keysym.sym] = false;
		return true;
	});

	window->setWindowEventCallback(SDL_WINDOWEVENT_RESIZED, [&](SDL_Event const& event) {
		width = event.window.data1;
		height = event.window.data2;
		glViewport(0, 0, width, height);
		cameraProjection->setAspect(float(width / height));
		return true;
	});

	//	Start main loop
	(*mainLoop)();

    return EXIT_SUCCESS;
}
