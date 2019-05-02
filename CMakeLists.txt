###
### CMakeLists.txt
### Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
###
cmake_minimum_required(VERSION 3.13.0)

project(
	proceduralCity
	VERSION 0.1.2
	LANGUAGES C CXX
	DESCRIPTION "Procedurální město - Bakalářská práce, Daniel Dolejška, FIT VUTBR 2018"
	)
	
# C++ Compiler Settings
set(CMAKE_CXX_STANDARD 14)
#target_compile_options(foo PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_FLAGS}>")
#target_compile_options(foo PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_FLAGS}>")

# C Compiler Settings
set(CMAKE_C_STANDARD 99)

# Directory setup
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRDPARTY_DIR ${ROOT_DIR}/3rd-party)
set(LIB_DIR ${ROOT_DIR}/lib)
set(INC_DIR ${ROOT_DIR}/include)
set(BIN_DIR ${ROOT_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules" "${THIRDPARTY_DIR}/CMakeModules")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR})

###
# Define files
###
set(SOURCES
	src/proceduralCity/main.cpp
	src/proceduralCity/Application/Application.cpp
	src/proceduralCity/Application/Renderer.cpp
	src/proceduralCity/Application/ShaderManager.cpp
	src/proceduralCity/Application/ShaderLoader.cpp
	src/proceduralCity/Infrastructure/Building.cpp
	src/proceduralCity/Infrastructure/BuildingPart.cpp
	src/proceduralCity/Infrastructure/Parcel.cpp
	src/proceduralCity/Infrastructure/Street.cpp
	src/proceduralCity/Infrastructure/StreetMap.cpp
	src/proceduralCity/Infrastructure/StreetZone.cpp
	src/proceduralCity/Terrain/Builder.cpp
	src/proceduralCity/Terrain/Generator.cpp
	src/proceduralCity/Terrain/HeightMap.cpp
	src/proceduralCity/Terrain/Chunk.cpp
	src/proceduralCity/Terrain/Map.cpp
	src/proceduralCity/Utils/Curve.cpp
	src/proceduralCity/Utils/QuadTree.cpp
	src/proceduralCity/Utils/QuadTreeNode.cpp
	src/proceduralCity/Utils/RectBounds.cpp
	)

set(INCLUDES
	src/proceduralCity/Application/Application.h
	src/proceduralCity/Application/IRenderableBase.h
	src/proceduralCity/Application/IRenderableArray.h
	src/proceduralCity/Application/IRenderableElementArray.h
	src/proceduralCity/Application/IRenderableCollection.h
	src/proceduralCity/Application/Renderer.h
	src/proceduralCity/Application/ShaderLoader.h
	src/proceduralCity/Application/ShaderManager.h
	src/proceduralCity/Infrastructure/Structs/BuildingPartVertex.h
	src/proceduralCity/Infrastructure/Structs/ParcelIndex.h
	src/proceduralCity/Infrastructure/Structs/ParcelVertex.h
	src/proceduralCity/Infrastructure/Structs/StreetIntersection.h
	src/proceduralCity/Infrastructure/Structs/StreetNarrowPair.h
	src/proceduralCity/Infrastructure/Structs/StreetSegment.h
	src/proceduralCity/Infrastructure/Structs/StreetSegmentIntersection.h
	src/proceduralCity/Infrastructure/Structs/StreetVertex.h
	src/proceduralCity/Infrastructure/Building.h
	src/proceduralCity/Infrastructure/BuildingPart.h
	src/proceduralCity/Infrastructure/Parcel.h
	src/proceduralCity/Infrastructure/Street.h
	src/proceduralCity/Infrastructure/StreetMap.h
	src/proceduralCity/Infrastructure/StreetZone.h
	src/proceduralCity/Terrain/Structs/ChunkVertex.h
	src/proceduralCity/Terrain/Structs/ChunkIndex.h
	src/proceduralCity/Terrain/Fwd.h
	src/proceduralCity/Terrain/Builder.h
	src/proceduralCity/Terrain/Generator.h
	src/proceduralCity/Terrain/HeightMap.h
	src/proceduralCity/Terrain/Chunk.h
	src/proceduralCity/Terrain/Map.h
	src/proceduralCity/Utils/Curve.h
	src/proceduralCity/Utils/functions.h
	src/proceduralCity/Utils/IHasRectBounds.h
	src/proceduralCity/Utils/QuadTree.h
	src/proceduralCity/Utils/QuadTreeNode.h
	src/proceduralCity/Utils/RectBounds.h
	)

file(GLOB_RECURSE SHADERS LIST_DIRECTORIES false RELATIVE ${ROOT_DIR} *.shader)

###
# Setup source groups for Visual Studio project file
###
function(assign_source_group)
	foreach(filename IN ITEMS ${ARGV})
		get_filename_component(filepath "${filename}" PATH)
		string(REPLACE "/" "\\" group_name "${filepath}")
		source_group("${group_name}" FILES "${filename}")
	endforeach()
endfunction(assign_source_group)

assign_source_group(${SOURCES})
assign_source_group(${INCLUDES})
assign_source_group(${SHADERS})

###
# Define packages
###
find_package(ArgumentViewer REQUIRED)
find_package(BasicCamera REQUIRED)
find_package(Vars REQUIRED)
find_package(geGL REQUIRED)
find_package(glm REQUIRED)
find_package(imguiSDL2OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2CPP REQUIRED)

find_package(FreeImage)
find_package(FreeImagePlus)

###
# Define targets
# CMake recognises non-build files itself
###
add_executable(proceduralCity ${SOURCES} ${INCLUDES} ${SHADERS})

target_include_directories(proceduralCity PUBLIC ${INC_DIR})
target_include_directories(proceduralCity PUBLIC ${ROOT_DIR}/src/proceduralCity)

target_link_libraries(proceduralCity
	SDL2::SDL2
	SDL2::SDL2main
	SDL2CPP::SDL2CPP
	BasicCamera::BasicCamera
	ArgumentViewer::ArgumentViewer
	imguiSDL2OpenGL::imguiSDL2OpenGL
	geGL::geGL
	Vars::Vars
	FreeImage
	FreeImagePlus
	)
	
# Build options
option(${PROJECT_NAME}_BUILD_TESTS "toggle building of unit tests" OFF)
if(${PROJECT_NAME}_BUILD_TESTS)
	enable_testing()

	add_library(proceduralCityTests STATIC ${SOURCES} ${INCLUDES})

	target_include_directories(proceduralCityTests PUBLIC ${INC_DIR})
	target_include_directories(proceduralCityTests PUBLIC ${ROOT_DIR}/src/proceduralCity)

	target_link_libraries(proceduralCityTests
		SDL2::SDL2
		SDL2::SDL2main
		SDL2CPP::SDL2CPP
		BasicCamera::BasicCamera
		ArgumentViewer::ArgumentViewer
		imguiSDL2OpenGL::imguiSDL2OpenGL
		geGL::geGL
		Vars::Vars
	)

	add_subdirectory(tests)
	add_test(NAME InfrastructureTests COMMAND tests_infrastructure)

	add_custom_target(tests COMMAND ${CMAKE_CTEST_COMMAND}
		--force-new-ctest-process
		--verbose
		--output-on-failure
	)
	add_dependencies(tests
		tests_infrastructure
	)
endif()