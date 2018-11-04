###
### CMakeLists.txt
### Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
###
cmake_minimum_required(VERSION 3.13.0)

project(
	proceduralCity_tests
	VERSION 0.1.0
	LANGUAGES CXX
	DESCRIPTION "Procedurální město - Testy implementace"
	)
	
# C++ Compiler Settings
set(CMAKE_CXX_STANDARD 14)

###
# Define files
###
set(TESTS_BASE
	catch.hpp
	TestsMain.cpp
)

set(INFRASTRUCTURE_TESTS
	Infrastructure/StreetNode.cpp
)

###
# Build includes
###
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/proceduralCity)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

###
# Define infrastructure tests
###
add_executable(tests_infrastructure ${TESTS_BASE} ${INFRASTRUCTURE_TESTS})
target_link_libraries(tests_infrastructure proceduralCityTests)
