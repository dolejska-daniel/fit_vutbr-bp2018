///
/// @file ShaderManager.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///

#pragma once

#include <string>

#include "Application.h"


///
/// @brief
///
namespace Application
{
	///
	/// @brief
	///
	class ShaderManager
	{
	public:
		static void init(const std::string filename);
		static void attach();
		static void detach();

		static ShaderSources sources;
		static unsigned int program;
		static unsigned int vs;
		static unsigned int fs;

	private:
		ShaderManager() {}

		static void compile(GLenum type, std::string source, unsigned int* shader_id);
	};
}
