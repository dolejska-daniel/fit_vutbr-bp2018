@ECHO OFF

:input_BuildThirdParty
SET /p buildThirdParty=Do you wish to build 3rd party libraries [y/n]?: 
IF /I "%buildThirdParty%"=="Y" GOTO yes_BuildThirdParty
IF /I "%buildThirdParty%"=="N" GOTO no_BuildThirdParty
GOTO input_BuildThirdParty
:yes_BuildThirdParty
cd ./3rd-party/
CALL ./run_cmake.bat
cd ../../
IF errorlevel 1 (
	EXIT /b %errorlevel%
)
:no_BuildThirdParty

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
ECHO Configuring main project...
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_PREFIX_PATH=./lib/cmake -DCMAKE_INSTALL_PREFIX=../ ../

IF errorlevel 1 (
	ECHO(
	ECHO CMake configuration FAILED!
	ECHO(
	PAUSE
	EXIT /b %errorlevel%
)

ECHO(
ECHO Building main project...
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
