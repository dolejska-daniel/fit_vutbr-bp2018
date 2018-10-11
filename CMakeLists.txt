###
### CMakeLists.txt
### Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
###

cmake_minimum_required(VERSION 3.10.2)

project(
	proceduralCity
	VERSION 0.1
	LANGUAGES C CXX
	DESCRIPTION "Procedurální město - Bakalářská práce, Daniel Dolejška, FIT VUTBR 2018"
	)
	
# C++ Compiler Settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # set global flags
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra) # set debug flags
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE}) # set release flags

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_COMPILER_FLAGS ${CMAKE_CXX_COMPILER_FLAGS} -g -O0)
else()
    set(CMAKE_CXX_COMPILER_FLAGS ${CMAKE_CXX_COMPILER_FLAGS} -O3 -Os)
endif()

# C Compiler Settings
set(CMAKE_C_STANDARD 99)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRDRDPARTY_DIR ${ROOT_DIR}/3rd-party)
set(LIB_DIR ${ROOT_DIR}/lib)
set(INC_DIR ${ROOT_DIR}/include)
set(BIN_DIR ${ROOT_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR})

set(CMAKE_DEBUG_POSTFIX			        "d"  CACHE STRING "add a postfix, usually d on windows")
set(CMAKE_RELEASE_POSTFIX		        ""   CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_RELEASEWITHDEBINFO_POSTFIX	"rd" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_MINSIZEREL_POSTFIX            "s"  CACHE STRING "add a postfix, usually empty on windows")

set(SOURCES
	src/proceduralCity/main.cpp
	src/proceduralCity/Application/ShaderManager.cpp
	src/proceduralCity/Application/ShaderLoader.cpp
	src/proceduralCity/Terrain.cpp
	)

set(INCLUDES
	src/proceduralCity/Application/Application.h
	src/proceduralCity/Application/ShaderLoader.h
	src/proceduralCity/Application/ShaderLoader.h
	src/proceduralCity/Terrain.h
	)

find_package(SDL2 REQUIRED PATHS ${ROOT_DIR})
find_package(SDL2CPP REQUIRED PATHS ${ROOT_DIR})
find_package(geGL REQUIRED PATHS ${ROOT_DIR})
find_package(BasicCamera REQUIRED PATHS ${ROOT_DIR})
find_package(imguiSDL2OpenGL REQUIRED PATHS ${ROOT_DIR})
find_package(glm REQUIRED PATHS ${ROOT_DIR})

include_directories(${INC_DIR})
link_directories(${BIN_DIR})
link_directories(${LIB_DIR})

add_executable(proceduralCity ${SOURCES} ${INCLUDES})

target_link_libraries(proceduralCity SDL2::SDL2 SDL2::SDL2main SDL2CPP::SDL2CPP BasicCamera::BasicCamera geGL::geGL imguiSDL2OpenGL::imguiSDL2OpenGL)

