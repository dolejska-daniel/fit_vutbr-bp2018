///
/// @file main.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <stdio.h>
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

using namespace glm;
using namespace ge::gl;
using namespace vars;
using namespace argumentViewer;
using namespace Application;
using namespace Terrain;

std::map<SDL_Keycode, bool> keyDown;


///
/// @brief
///
/// @param argc		number of arguments
/// @param argv[]	array of arguments
///
int main(int argc, char* argv[])
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
	vars.addFloat( "terrain.amplitude",		args.getf32("--terrain-amplitude",		3.f,	""));
	vars.addFloat( "terrain.frequency",		args.getf32("--terrain-frequency",		2.f,	""));
	vars.addFloat( "terrain.persistence",	args.getf32("--terrain-persistence",	0.5f,	""));
	vars.addFloat( "terrain.lacunarity",	args.getf32("--terrain-lacunarity",		1.25f,	""));
	vars.addUint32("terrain.octaves",		args.getu32("--terrain-octaves",		3,		"Number of noises from which is the terrain going to be generated"));
	vars.addUint32("terrain.detail",		args.getu32("--terrain-detail",			1,		"Default level of object detail"));

	vars.addUint32("terrain.map.width",		args.getu32("--terrain-map-width",	1, "Terrain map width (count of chunks)"));
	vars.addUint32("terrain.map.height",	args.getu32("--terrain-map-height", 1, "Terrain map height (count of chunks) in 2D (depth/length in 3D)"));

	vars.addUint32("terrain.chunk.width",	args.getu32("--terrain-chunk-width",	64, "Chunk width"));
	vars.addUint32("terrain.chunk.height",	args.getu32("--terrain-chunk-height",	64, "Terrain chunk height in 2D (depth/length in 3D)"));

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
	ge::gl::init();

	std::cerr << "Using " << glGetString(GL_VERSION) << std::endl;

	//	Initialize ShaderManager
	ShaderManager* shaders = new ShaderManager(vars);
	shaders->Use("Phong");

	// Enable debug mode
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//	Setup main camera
	auto cameraProjection = std::make_shared<basicCamera::PerspectiveCamera>(glm::radians(45.f), width / height, 0.1f, std::numeric_limits<float>::infinity());
	auto freeLook = std::make_shared<basicCamera::FreeLookCamera>();
	freeLook->setPosition(vec3(1, 1, 4));


	Map* map = Generator::GenerateMap(vars);

	std::cerr << "Setting up VBOs and VAOs" << std::endl;

	int mapWidth = vars.getUint32("terrain.map.width");
	int mapHeight = vars.getUint32("terrain.map.height");

	std::shared_ptr<VertexArray>* vertexArrays = new std::shared_ptr<VertexArray>[mapWidth * mapHeight];
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			Chunk* chunk = map->chunks[y * mapWidth + x];

			auto vertexArray = vertexArrays[y * width + x] = std::make_shared<VertexArray>();
			vertexArray->bind();

			auto vertexBuffer = std::make_shared<Buffer>(chunk->GetVerticesSize(), chunk->GetVertices());
			vertexBuffer->bind(GL_ARRAY_BUFFER);

			//	POSITION
			vertexArray->addAttrib(vertexBuffer, 0, 3, GL_FLOAT, 6 * sizeof(float));
			//	NORMAL
			vertexArray->addAttrib(vertexBuffer, 1, 3, GL_FLOAT, 6 * sizeof(float), 3 * sizeof(float));

			auto elementBuffer = std::make_shared<Buffer>(chunk->GetIndicesSize(), chunk->GetIndices());
			elementBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
			vertexArray->addElementBuffer(elementBuffer);

			std::cerr << "Number of buffers:" << vertexArray->getNofBuffers() << std::endl;
		}
	}




	/*
	unsigned int hmapTex;
	glGenTextures(1, &hmapTex);
	glBindTexture(GL_TEXTURE_2D, hmapTex);
	
	//	Texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//	Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	Texture border
	glm::vec4 hmapBorder(0.f, 0.f, 0.f, 0.f);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float *)&hmapBorder);

	HeightMap map = HeightMap(vars, 32, 32);
	float *hmap = map.GetMap();
	glm::vec3 *hmapTexture = new glm::vec3[map.GetWidth() * map.GetHeight()];
	for (int y = 0; y < map.GetHeight(); y++)
	{
		for (int x = 0; x < map.GetWidth(); x++)
		{
			int index = y * map.GetWidth() + x;
			hmapTexture[index].r = hmapTexture[index].g = hmapTexture[index].b = glm::mix(0, 1, hmap[index]);
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, map.GetWidth(), map.GetHeight(), 0, GL_RGB, GL_FLOAT, hmapTexture);
	*/


	auto streetMap = Infrastructure::StreetMap();
	Renderer renderer = Renderer(vars);

	std::cerr << "Callback" << std::endl;
	//	Drawing
    mainLoop->setIdleCallback([&](){

		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int a = 0; a < 3; ++a)
			freeLook->move(a, float(keyDown["d s"[a]] - keyDown["acw"[a]]) * .25f);

		vec3 cameraPosition = freeLook->getPosition();
		mat4 projectionMatrix = cameraProjection->getProjection();
		mat4 viewMatrix = freeLook->getView();
		mat4 modelMatrix(1);

		for (int y = 0; y < mapHeight; y++)
		{
			for (int x = 0; x < mapWidth; x++)
			{
				auto vertexArray = vertexArrays[y * mapWidth + x];
				vertexArray->bind();

				shaders->Use("Phong");
				shaders->activeProgram->set3fv("lightPosition_worldspace", &cameraPosition[0]);
				shaders->activeProgram->set3fv("cameraPosition_worldspace", &cameraPosition[0]);
				shaders->activeProgram->setMatrix4fv("projectionMatrix", &projectionMatrix[0][0]);
				shaders->activeProgram->setMatrix4fv("viewMatrix", &viewMatrix[0][0]);
				shaders->activeProgram->setMatrix4fv("modelMatrix", &modelMatrix[0][0]);
				glDrawElements(GL_TRIANGLES, vertexArray->getElement()->getSize(), GL_UNSIGNED_INT, (const void*)0);
				//glMultiDrawElements(GL_TRIANGLES,);
				// glMultiDrawElementsIndirect()



				/*
				shaders->Use("Geometry_Normals");
				shaders->activeProgram->setMatrix4fv("projectionMatrix", &projectionMatrix[0][0]);
				shaders->activeProgram->setMatrix4fv("viewMatrix", &viewMatrix[0][0]);
				shaders->activeProgram->setMatrix4fv("modelMatrix", &modelMatrix[0][0]);
				glDrawElements(GL_TRIANGLES, vertexArray->getElement()->getSize(), GL_UNSIGNED_INT, (const void*)0);
				*/
			}
		}

		if (keyDown['x'] == true)
		{
			streetMap.BuildStep();
		}
		else if (keyDown['y'] == true)
		{
			printf("Number of streets: %d\n", streetMap.ReadStreets().size());
		}

		auto streets = streetMap.ReadStreets();
		for (size_t i = 0; i < streets.size(); i++)
		{
			renderer.Render(streets[i]);
		}

		glFinish();
        window->swap();
    });

	//	Camera rotation
	window->setEventCallback(SDL_MOUSEMOTION, [&](SDL_Event const &event) {
		if (event.motion.state & SDL_BUTTON_LMASK)
		{
			//	Trap mouse
			SDL_SetRelativeMouseMode(SDL_TRUE);
			
			auto const sensitivity = 0.0025f;
			auto const xrel = static_cast<float>(event.motion.xrel);
			auto const yrel = static_cast<float>(event.motion.yrel);

			freeLook->setAngle(1, freeLook->getAngle(1) + xrel * sensitivity);
			freeLook->setAngle(0, freeLook->getAngle(0) + yrel * sensitivity);

			return  true;
		}
		else
		{
			//	Free mouse
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}

		return false;
	});

	window->setEventCallback(SDL_KEYDOWN, [&](SDL_Event const &event) {
		keyDown[event.key.keysym.sym] = true;
		return true;
	});

	window->setEventCallback(SDL_KEYUP, [&](SDL_Event const &event) {
		keyDown[event.key.keysym.sym] = false;
		return true;
	});

	//	Start main loop
	(*mainLoop)();

    return EXIT_SUCCESS;
}
