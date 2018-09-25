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

#include "Application/Application.h"
#include "Application/ShaderManager.h"

using namespace std;
using namespace ge::gl;
using namespace Application;


#define TRIANGLE_RENDER


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

#ifdef TRIANGLE_RENDER

#pragma region Triangle Vertices

	float vertices[] = {
		// x,    y,   z,   r,   g,   b
		-.5f, -.5f, 0.f, 1.f, 0.f, 0.f,
		 .5f, -.5f, 0.f, 0.f, 1.f, 0.f,
		 .5f,  .5f, 0.f, 0.f, 0.f, 1.f
	};

#pragma endregion

	//	Create, bind and fill vertex buffer on GPU
	unsigned int vertexBufferObject;
	glCreateBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER,  6 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

	//	Define vertex attribute - POSITION (0)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glBindAttribLocation(ShaderManager::program, 0, "position");

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*) (3 * sizeof(float)));

	float param = 0;
	float paramInc = -0.005f;

	float yParam = 0;
	float yParamInc = 0;

	float cameraDistance = 2;

#else

#pragma region Cube Vertices

	static const GLfloat vertices[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

#pragma endregion

	//	Create, bind and fill vertex buffer on GPU
	unsigned int vertexBufferObject;
	glCreateBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * 9 * sizeof(float), vertices, GL_STATIC_DRAW);

	//	Define vertex attribute - POSITION (0)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glBindAttribLocation(ShaderManager::program, 0, "position");


#pragma region Cube Vertex Colors

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

#pragma endregion

	unsigned int colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	
	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	float param = 0;
	float paramInc = -0.01f;

	float yParam = 0;
	float yParamInc = 0.005f;

	float cameraDistance = 4;

#endif

	//	Define MVP matrix
	unsigned int mvpMatrix_uniformLocation = glGetUniformLocation(ShaderManager::program, "mvpMatrix");

	glm::vec3 cameraPosition(0, 0, 1);
	glm::vec3 cameraDirection(0);

	//	Drawing
    mainLoop->setIdleCallback([&](){

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		param += paramInc;
		yParam += yParamInc;

		cameraPosition.x = cameraDistance * cos(param);
		cameraPosition.z = cameraDistance * sin(param);
		cameraPosition.y = cameraDistance * sin(yParam);

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

#ifdef TRIANGLE_RENDER
		glDrawArrays(GL_TRIANGLES, 0, 3);
#else
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
#endif

		glFinish();

        window->swap();
    });

	//	Start main loop
	(*mainLoop)();

	ShaderManager::detach();

    return EXIT_SUCCESS;
}
