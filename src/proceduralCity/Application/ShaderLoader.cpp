///
/// @file ShaderLoader.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

#include "Application.h"
#include "ShaderLoader.h"

using namespace Application;


std::string ShaderLoader::filename; ///< 

std::unordered_map<std::string, ShaderType> ShaderLoader::typeMapping ///< 
{
	std::pair<std::string, ShaderType>("vertex",   ShaderType::VERTEX),
	std::pair<std::string, ShaderType>("fragment", ShaderType::FRAGMENT),
};

///
/// @brief
///
/// @param
///
std::ifstream ShaderLoader::getStream()
{
	std::string filepath = "../res/shaders/" + getSourceFilename();
	std::cerr << "Reading shaders from: " << filepath << std::endl;
	return std::ifstream(filepath, std::ifstream::in);
}

///
/// @brief
///
ShaderSources ShaderLoader::parse()
{
	std::string line;
	std::stringstream sources[2];
	ShaderType type = ShaderType::NONE;

	std::ifstream stream = getStream();
	if (stream.is_open() == false)
		std::cerr << "Failed to open shaders file: " << strerror(errno) << std::endl;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			//	FIXME: Type might not be present
			type = typeMapping[line.substr(8)];
			continue;
		}

		sources[(int)type] << line << std::endl;
	}

	return {
		sources[0].str(),
		sources[1].str(),
	};
}
