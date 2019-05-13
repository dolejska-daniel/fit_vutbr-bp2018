///
/// @file ShaderLoader.cpp
/// @brief Obsahuje definice funkcí z namespace Application::ShaderLoader.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <iostream> // 

#include <fstream> // std::ifstream
#include <string> // std::string
#include <sstream> // std::stringstream
#include <Application/Application.h>
#include <Application/ShaderLoader.h>

using namespace Application;


const std::unordered_map<std::string, ShaderType> ShaderLoader::Types = {
	std::pair<std::string, ShaderType>("vertex",   ShaderType::VERTEX),
	std::pair<std::string, ShaderType>("geometry", ShaderType::GEOMETRY),
	std::pair<std::string, ShaderType>("fragment", ShaderType::FRAGMENT),
};

std::unordered_map<std::string, ShaderSources> ShaderLoader::Files = {};


std::ifstream ShaderLoader::OpenFile(std::string const& filepath)
{
	std::cerr << "Reading shaders from: " << filepath << std::endl;
	return std::ifstream(filepath.c_str(), std::ifstream::in);
}

void ShaderLoader::CloseFile(std::ifstream &file)
{
	file.close();
}

ShaderSources ShaderLoader::GetShaderSources(std::string const& filename)
{
	//	Pokus o vyhledání z již zpracovaných zdrojáků
	const auto existingSources = ShaderLoader::Files.find(filename);
	if (existingSources != ShaderLoader::Files.end())
		//	Zdrojáky byly nalezeny
		return existingSources->second;

	std::string line;
	const auto sources = new std::stringstream[Types.size()];
	auto type = ShaderType::NONE;

	auto stream = OpenFile(Vars.getString("resources.dir") + "/shaders/" + filename + ".shader");
	if (!stream.is_open())
		//	Stream se nepodařilo otevřít
		throw std::invalid_argument("Shader file with given name was not found.");

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			//	Řádek identifikující typ shaderu
			auto typeSearch = ShaderLoader::Types.find(line.substr(8));
			if (typeSearch == ShaderLoader::Types.end())
				//	Typ ze souboru není podporován
				throw std::out_of_range("Shader file contains unsupported shader type.");

			type = typeSearch->second;
			continue;
		}
		
		if (line.find("//") == 0)
		{
			//	Komentář ve zdrojáku
			continue;
		}

		sources[static_cast<int>(type)] << line << std::endl;
	}

	ShaderSources result {
		sources[0].str(),
		sources[1].str(),
		sources[2].str(),
	};

	delete[] sources;
	return result;
}
