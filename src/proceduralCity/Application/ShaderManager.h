///
/// @file ShaderManager.h
/// @brief Obsahuje deklaraci pro tøídu ShaderManager.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <string>
#include <unordered_map>
#include <geGL/Program.h>

#include "Application.h"


namespace Application
{
	///
	/// @brief
	///
	class ShaderManager
	{
	public:
		std::shared_ptr<ge::gl::Program> activeProgram;

		ShaderManager(vars::Vars& vars);
		~ShaderManager();

		void Use(const std::string programName);

	private:
		vars::Vars& vars;
		std::unordered_map<std::string, std::shared_ptr<ge::gl::Program>> programs;

		void BindProgram(std::shared_ptr<ge::gl::Program> program);
	};
}
