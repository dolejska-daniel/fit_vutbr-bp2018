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

#include "Application/Application.h"
#include "Application/ShaderManager.h"

using namespace std;
using namespace ge::gl;
using namespace Application;

///
/// @brief
///
/// @param argc		number of arguments
/// @param argv[]	array of arguments
///
int main(int argc, char* argv[])
{
	//	Create main loop & window
	auto mainLoop = make_shared<sdl2cpp::MainLoop>();
	auto window   = make_shared<sdl2cpp::Window  >(640, 480);
    window->createContext("rendering");
    mainLoop->addWindow("mainWindow", window);

	//	Initialize geGL
	ge::gl::init();

	std::cerr << "Using " << glGetString(GL_VERSION) << std::endl;

	float vertices[6] = {
		-.5f, -.5f,
		 .5f, -.5f,
		 .5f,  .5f
	};

	unsigned int vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//	Create, bind and fill vertex buffer on GPU
	unsigned int vertexBufferObject;
	glCreateBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

	//	Define vertex attribute - POSITION (0)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	glBindAttribLocation(ShaderManager::program, 0, "position");

	//	Initialize ShaderManager
	ShaderManager::init("Default");
	ShaderManager::attach();

	//	Drawing
    mainLoop->setIdleCallback([&](){

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glFinish();

        window->swap();
    });

	//	Start main loop
	(*mainLoop)();

	ShaderManager::detach();

    return EXIT_SUCCESS;
}
