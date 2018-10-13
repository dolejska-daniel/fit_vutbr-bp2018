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

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>

#include <BasicCamera/PerspectiveCamera.h>
#include <BasicCamera/FreeLookCamera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ArgumentViewer/ArgumentViewer.h>
#include <Vars/Vars.h>

#include "Application/Application.h"
#include "Application/ShaderManager.h"
#include "Terrain/Generator.h"
#include "Terrain/Map.h"
#include "Terrain/Chunk.h"

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

	uint32_t width = 1280;
	uint32_t height = 800;

	//	Create main loop & window
	auto mainLoop = std::make_shared<sdl2cpp::MainLoop>();
	auto window   = std::make_shared<sdl2cpp::Window  >(width, height);
    window->createContext("rendering", 450u, sdl2cpp::Window::CORE, sdl2cpp::Window::DEBUG);
    mainLoop->addWindow("mainWindow", window);

	//	Initialize geGL
	ge::gl::init();

	std::cerr << "Using " << glGetString(GL_VERSION) << std::endl;

	//	Initialize ShaderManager
	ShaderManager::init("Default");
	ShaderManager::attach();

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	//	Define MVP matrix
	int mvpMatrix_uniformLocation = glGetUniformLocation(ShaderManager::program, "mvpMatrix");

	//	Define MVP matrix
	int lightPosition_uniformLocation = glGetUniformLocation(ShaderManager::program, "lightPosition_worldspace");
	glUniform3f(lightPosition_uniformLocation, 2.f, 2.f, 0.f);

	//	Setup cameras
	int cameraPosition_uniformLocation = glGetUniformLocation(ShaderManager::program, "cameraPosition_worldspace");
	auto cameraProjection = std::make_shared<basicCamera::PerspectiveCamera>(glm::radians(45.f), width / height, 0.1f, std::numeric_limits<float>::infinity());
	auto freeLook = std::make_shared<basicCamera::FreeLookCamera>();

	freeLook->setPosition(vec3(1, 1, 4));


	Map* map = Generator::GenerateMap(vars);
	Chunk* chunk = map->chunks[0];
	

	std::cerr << "Setting up OpenGL" << std::endl;
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	std::cerr << "Setting up VBO" << std::endl;
	//	Create, bind and fill vertex buffer on GPU
	unsigned int vbo;
	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, chunk->GetVerticesSize(), chunk->GetVertices(), GL_STATIC_DRAW);
	printf("Vertices size: %d\n", chunk->GetVerticesSize());

	//	Define vertex attribute - POSITION (0)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	//	Define vertex attribute - NORMAL (1)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float)));

	std::cerr << "Setting up IBO" << std::endl;
	unsigned int ibo;
	glCreateBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->GetIndicesSize(), chunk->GetIndices(), GL_STATIC_DRAW);
	printf("Vertices size: %d\n", chunk->GetIndicesSize());

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

	/*
	auto vertexArray = std::make_shared<VertexArray>();

	auto vertexBuffer = std::make_shared<Buffer>(t.GetVerticesSize());
	vertexBuffer->setData(vertices);

	//	POSITION
	vertexArray->addAttrib(vertexBuffer, 0, 3, GL_FLOAT, 6 * sizeof(float));
	//	NORMAL
	vertexArray->addAttrib(vertexBuffer, 1, 3, GL_FLOAT, 6 * sizeof(float), 3 * sizeof(float));

	auto indexBuffer = std::make_shared<Buffer>(t.GetIndicesCount());
	vertexBuffer->setData(indices);

	vertexArray->addElementBuffer(indexBuffer);
	*/


	std::cerr << "Callback" << std::endl;
	//	Drawing
    mainLoop->setIdleCallback([&](){

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int a = 0; a < 3; ++a)
			freeLook->move(a, float(keyDown["d s"[a]] - keyDown["acw"[a]]) * .25f);

		vec3 cameraPosition = freeLook->getPosition();
		glUniform3f(lightPosition_uniformLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glUniform3f(cameraPosition_uniformLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		mat4 projectionMatrix = cameraProjection->getProjection();
		mat4 viewMatrix = freeLook->getView();
		mat4 modelMatrix(1);

		mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(mvpMatrix_uniformLocation, 1, GL_FALSE, &mvpMatrix[0][0]);


		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, chunk->GetIndicesSize(), GL_UNSIGNED_INT, (const void*)0);

		/*
		vertexArray->bind();
		indexBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, t.GetIndicesSize(), GL_UNSIGNED_INT, (const void*)0);
		*/

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

			freeLook->setAngle(
				1, freeLook->getAngle(1) + xrel * sensitivity);
			freeLook->setAngle(
				0, freeLook->getAngle(0) + yrel * sensitivity);

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

	ShaderManager::detach();

    return EXIT_SUCCESS;
}
