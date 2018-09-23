cmake_minimum_required(VERSION 3.10.2)

project(
	proceduralCity
	VERSION 0.1
	LANGUAGES C CXX
	DESCRIPTION "Procedurální město - Bakalářská práce, Daniel Dolejška, FIT VUTBR 2018"
	)
	
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

#add_compile_options(-W)
add_compile_options(-Wall)
#add_compile_options(-Wextra)
#add_compile_options(-Wconversion)
add_compile_options(-pedantic)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRDRDPARTY_DIR ${ROOT_DIR}/3rd-party)
set(LIB_DIR ${ROOT_DIR}/lib)
set(INC_DIR ${ROOT_DIR}/include)
set(BIN_DIR ${ROOT_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR})

set(CMAKE_DEBUG_POSTFIX			"d" CACHE STRING "add a postfix, usually d on windows")
set(CMAKE_RELEASE_POSTFIX		"" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_RELEASEWITHDEBINFO_POSTFIX	"rd" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_MINSIZEREL_POSTFIX		"s" CACHE STRING "add a postfix, usually empty on windows")

set(SOURCES
	src/proceduralCity/main.cpp
	)

set(INCLUDES
	)

find_package(SDL2 REQUIRED PATHS ${ROOT_DIR})
find_package(SDL2CPP REQUIRED PATHS ${ROOT_DIR})
find_package(geGL REQUIRED PATHS ${ROOT_DIR})
find_package(BasicCamera REQUIRED PATHS ${ROOT_DIR})
find_package(imguiDormon REQUIRED PATHS ${ROOT_DIR})
find_package(imguiOpenGLDormon REQUIRED PATHS ${ROOT_DIR})
find_package(imguiSDL2Dormon REQUIRED PATHS ${ROOT_DIR})
find_package(imguiSDL2OpenGL REQUIRED PATHS ${ROOT_DIR})
#find_package(glm REQUIRED PATHS ${ROOT_DIR})

include_directories(${INC_DIR})
link_directories(${BIN_DIR})
link_directories(${LIB_DIR})

add_executable(proceduralCity ${SOURCES} ${INCLUDES})

target_link_libraries(proceduralCity
	debug geGLd
	optimized geGL)

target_link_libraries(proceduralCity
	debug SDL2CPPd
	optimized SDL2CPP)

target_link_libraries(proceduralCity
	debug BasicCamerad
	optimized BasicCamera)

target_link_libraries(proceduralCity
	debug SDL2d
	optimized SDL2)

target_link_libraries(proceduralCity
	debug SDL2maind
	optimized SDL2main)

target_link_libraries(proceduralCity
	debug imguiDormond
	optimized imguiDormon)

target_link_libraries(proceduralCity
	debug imguiSDL2Dormond
	optimized imguiSDL2Dormon)

target_link_libraries(proceduralCity
	debug imguiOpenGLDormond
	optimized imguiOpenGLDormon)

target_link_libraries(proceduralCity
	debug imguiSDL2OpenGLd
	optimized imguiSDL2OpenGL)

#add_library(glm ${LIB_DIR})
#target_link_libraries(proceduralCity glm)
