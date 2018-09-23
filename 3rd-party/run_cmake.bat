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
ECHO Configuring...
cmake -G "Visual Studio 15 2017 Win64" ../

ECHO(
ECHO Building...
cmake --build ./

ECHO(
ECHO Completed!
PAUSE
