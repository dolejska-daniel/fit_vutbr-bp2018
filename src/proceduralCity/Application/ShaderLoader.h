///
/// @file ShaderLoader.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///

#pragma once

#include <string>
#include <unordered_map>

#include "Application.h"


///
/// @brief
///
namespace Application
{
	///
	/// @brief
	///
	class ShaderLoader
	{
	public:
		static std::ifstream getStream();
		static ShaderSources parse();

		static const std::string& getSourceFilename() { return filename; }
		static void setSourceFilename(const std::string& _filename) { filename = _filename + ".shader"; }

		static std::string filename;
		static std::unordered_map<std::string, ShaderType> typeMapping;

	private:
		ShaderLoader() {}
	};
}
