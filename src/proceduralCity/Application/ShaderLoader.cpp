///
/// @file ShaderLoader.cpp
/// @brief Obsahuje definice funkcí z namespace Application::ShaderLoader.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <iostream> // std::cerr
#include <fstream> // std::ifstream
#include <string> // std::string
#include <sstream> // std::stringstream
#include <Application/Application.h>
#include <Application/ShaderLoader.h>

using namespace Application;


const std::unordered_map<std::string, ShaderType> ShaderLoader::types = {
	std::pair<std::string, ShaderType>("vertex",   ShaderType::VERTEX),
	std::pair<std::string, ShaderType>("geometry", ShaderType::GEOMETRY),
	std::pair<std::string, ShaderType>("fragment", ShaderType::FRAGMENT),
};

std::unordered_map<std::string, ShaderSources> ShaderLoader::files = {};


std::ifstream ShaderLoader::OpenFile(const std::string filepath)
{
	std::cerr << "Reading shaders from: " << filepath << std::endl;
	return std::ifstream(filepath.c_str(), std::ifstream::in);
}

void ShaderLoader::CloseFile(std::ifstream file)
{
	file.close();
}

ShaderSources ShaderLoader::GetShaderSources(const vars::Vars& vars, const std::string filename)
{
	//	Pokus o vyhledání z již zpracovaných zdrojáků
	auto existingSources = ShaderLoader::files.find(filename);
	if (existingSources != ShaderLoader::files.end())
		//	Zdrojáky byly nalezeny
		existingSources->second;

	std::string line;
	std::stringstream *sources = new std::stringstream[types.size()];
	ShaderType type = ShaderType::NONE;

	std::ifstream stream = OpenFile(vars.getString("resources.dir") + "/shaders/" + filename + ".shader");
	if (stream.is_open() == false)
		//	Stream se nepodařilo otevřít
		throw std::invalid_argument("Shader file with given name was not found.");

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			//	Řádek identifikující typ shaderu
			auto typeSearch = ShaderLoader::types.find(line.substr(8));
			if (typeSearch == ShaderLoader::types.end())
				//	Typ ze souboru není podporován
				throw std::out_of_range("Shader file contains unsupported shader type.");

			type = typeSearch->second;
			continue;
		}
		else if (line.find("//") == 0)
		{
			//	Komentář ve zdrojáku
			continue;
		}

		sources[(int)type] << line << std::endl;
	}

	return {
		sources[0].str(),
		sources[1].str(),
		sources[2].str(),
	};
}
