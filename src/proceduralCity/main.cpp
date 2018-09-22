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

using namespace std;
using namespace ge::gl;

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
    auto window   = make_shared<sdl2cpp::Window  >();
    window->createContext("rendering");
    mainLoop->addWindow("mainWindow",window);

	//	Initialize geGL
    ge::gl::init();

	//	Drawing
    mainLoop->setIdleCallback([&](){
        glClearColor(1.f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//	Draw triangle
        glBegin(GL_TRIANGLES);
        glVertex2f(-.5f, -.5f);
        glVertex2f(-.5f,  .5f);
        glVertex2f( .5f,  .5f);
        glEnd();

		glFinish();

        window->swap();
    });

	//	Start main loop
	(*mainLoop)();

    return EXIT_SUCCESS;
}
