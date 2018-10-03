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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/perpendicular.hpp>

#include "Application/Application.h"
#include "Application/ShaderManager.h"

#include "bunny.h"

using namespace std;
using namespace ge::gl;
using namespace Application;


//#define TRIANGLE_RENDER


///
/// @brief
///
/// @param argc		number of arguments
/// @param argv[]	array of arguments
///
int main(int argc, char* argv[])
{
	uint32_t width = 640;
	uint32_t height = 480;

	//	Create main loop & window
	auto mainLoop = make_shared<sdl2cpp::MainLoop>();
	auto window   = make_shared<sdl2cpp::Window  >(width, height);
    window->createContext("rendering");
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

	unsigned int vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	//	Create, bind and fill vertex buffer on GPU
	unsigned int vertexBufferObject;
	glCreateBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 1048 * 6 * sizeof(float), &bunnyVertices[0], GL_STATIC_DRAW);

	//	Define vertex attribute - POSITION (0)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	//glBindAttribLocation(ShaderManager::program, 0, "position");

	//	Define vertex attribute - NORMAL (1)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*) (3 * sizeof(float)));

	//	Create, bind and fill index buffer on GPU
	unsigned int indexBufferObject;
	glCreateBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2092 * 3 * sizeof(unsigned int), &bunnyIndices[0], GL_STATIC_DRAW);

	std::cerr << "Vertices size:" << sizeof(bunnyVertices) << std::endl;
	std::cerr << "Indices size:" << sizeof(bunnyIndices) << std::endl;

	float param = 0;
	float paramInc = -0.01f;

	float yParam = 0;
	float yParamInc = 0.005f;

	float cameraDistance = 4;

	/*
	//	Define Light position
	int lightPosition_uniformLocation = glGetUniformLocation(ShaderManager::program, "lightPosition_worldspace");
	if (lightPosition_uniformLocation != -1)
	{
		glUniform3f(lightPosition_uniformLocation, 1.f, 2.f, 2.f);
	}
	*/

	//	Define MVP matrix
	int mvpMatrix_uniformLocation = glGetUniformLocation(ShaderManager::program, "mvpMatrix");

	//	Define MVP matrix
	int cameraPosition_uniformLocation = glGetUniformLocation(ShaderManager::program, "cameraPosition_worldspace");

	//	Define MVP matrix
	int lightPosition_uniformLocation = glGetUniformLocation(ShaderManager::program, "lightPosition_worldspace");
	glUniform3f(lightPosition_uniformLocation, 2.f, 2.f, 0.f);

	glm::vec3 cameraPosition(0, 0, 1);
	glm::vec3 cameraDirection(0);

	//	Drawing
    mainLoop->setIdleCallback([&](){

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		param += paramInc;
		yParam += yParamInc;

		cameraPosition.x = cameraDistance * cos(param);
		cameraPosition.z = cameraDistance * sin(param);
		cameraPosition.y = cameraDistance / 2 * sin(yParam);
		glUniform3f(cameraPosition_uniformLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		if (mvpMatrix_uniformLocation != -1)
		{
			glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.f), (float)width / (float)height, 0.1f, 100.0f);

			glm::mat4 viewMatrix = glm::lookAt(
				cameraPosition,		// the position of your camera, in world space
				cameraDirection,	// where you want to look at, in world space
				glm::vec3(0, 1, 0)
			);

			glm::mat4 modelMatrix(1);

			glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

			glUniformMatrix4fv(mvpMatrix_uniformLocation, 1, GL_FALSE, &mvpMatrix[0][0]);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glDrawElements(GL_TRIANGLES, 2092 * 3 * sizeof(unsigned int), GL_UNSIGNED_INT, (const void*)0);

		glFinish();

        window->swap();
    });

	//	Start main loop
	(*mainLoop)();

	ShaderManager::detach();

    return EXIT_SUCCESS;
}
