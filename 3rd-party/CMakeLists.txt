###
### CMakeLists.txt
### Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
###
cmake_minimum_required(VERSION 3.12.3)

project(
	3rd-party
	LANGUAGES C CXX
	)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PARENT_DIR ${ROOT_DIR}/..)

include(ExternalProject)
ExternalProject_Add(geGL
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
		-DCMAKE_PROJECT_geGL_INCLUDE:PATH=${PARENT_DIR}/include.cmake
		-DROOT_DIR:PATH=${PARENT_DIR}
		-DBUILD_SHARED_LIBS=ON

	GIT_REPOSITORY	https://github.com/dormon/geGL
	GIT_TAG			0c4550226bf8fc0c302a5d3f2d56fce005d6828e # working
	)

ExternalProject_Add(SDL2CPP
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
		-DCMAKE_PROJECT_SDL2CPP_INCLUDE:PATH=${PARENT_DIR}/include.cmake
		-DROOT_DIR:PATH=${PARENT_DIR}

	GIT_REPOSITORY	https://github.com/dormon/SDL2CPP
	GIT_TAG			master
	)

ExternalProject_Add(BasicCamera
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
		-DCMAKE_PROJECT_BasicCamera_INCLUDE:PATH=${PARENT_DIR}/include.cmake
		-DROOT_DIR:PATH=${PARENT_DIR}

	GIT_REPOSITORY	https://github.com/dormon/BasicCamera
	GIT_TAG			master
	)

ExternalProject_Add(imguiDormon
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

	GIT_REPOSITORY	https://github.com/dormon/imguiDormon
	GIT_TAG			master
	)

ExternalProject_Add(imguiSDL2Dormon
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

	GIT_REPOSITORY	https://github.com/dormon/imguiSDL2Dormon
	GIT_TAG			master
	)
add_dependencies(imguiSDL2Dormon imguiDormon)

ExternalProject_Add(imguiOpenGLDormon
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

	GIT_REPOSITORY	https://github.com/dormon/imguiOpenGLDormon
	GIT_TAG			master
	)
add_dependencies(imguiOpenGLDormon imguiDormon)

ExternalProject_Add(imguiSDL2OpenGL
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

	GIT_REPOSITORY	https://github.com/dormon/imguiSDL2OpenGl
	GIT_TAG			master
	)
add_dependencies(imguiSDL2OpenGL imguiDormon imguiSDL2Dormon imguiOpenGLDormon)

ExternalProject_Add(Vars
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

	GIT_REPOSITORY	https://github.com/dormon/Vars
	GIT_TAG			master
	)

ExternalProject_Add(MealyMachine
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

	GIT_REPOSITORY	https://github.com/dormon/MealyMachine
	GIT_TAG			master
	)

ExternalProject_Add(TxtUtils
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

	GIT_REPOSITORY	https://github.com/dormon/TxtUtils
	GIT_TAG			master
	)

ExternalProject_Add(ArgumentViewer
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

	GIT_REPOSITORY	https://github.com/dormon/ArgumentViewer
	GIT_TAG			master
	)
add_dependencies(ArgumentViewer MealyMachine TxtUtils)

ExternalProject_Add(SDL2
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

	GIT_REPOSITORY	https://github.com/SDL-mirror/SDL
	GIT_TAG			release-2.0.8
	)
add_dependencies(SDL2CPP SDL2)

ExternalProject_Add(glm
	PREFIX 		repos
	INSTALL_DIR	${PARENT_DIR}
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

	GIT_REPOSITORY	https://github.com/g-truc/glm
	GIT_TAG			0.9.9.2
	)
add_dependencies(BasicCamera glm)
