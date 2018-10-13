@ECHO OFF

IF NOT EXIST build (
	ECHO(
	ECHO Creating build directory.
	mkdir build
)

cd ./build

IF EXIST cmake_install.cmake (
	ECHO(
	ECHO Removing cmake_install.
	del cmake_install.cmake
)

IF EXIST CMakeCache.txt (
	ECHO(
	ECHO Removing CMakeCache.
	del CMakeCache.txt
)

ECHO(
ECHO Configuring third party libraries...
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_PREFIX_PATH=../../lib/cmake -DCMAKE_INSTALL_PREFIX=../../ ../

IF errorlevel 1 (
	ECHO(
	ECHO CMake configuration FAILED!
	ECHO(
	PAUSE
	EXIT /b %errorlevel%
)

ECHO(
ECHO Building third party libraries...
cmake --build ./

IF errorlevel 1 (
	ECHO(
	ECHO CMake configuration FAILED!
	ECHO(
	PAUSE
	EXIT /b %errorlevel%
)

ECHO(
ECHO Completed!
PAUSE
