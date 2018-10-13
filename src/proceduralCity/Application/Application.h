///
/// @file Application.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

#include <string>


///
/// @brief
///
namespace Application
{
	///
	/// @brief
	///
	enum class ShaderType
	{
		NONE		= -1,
		VERTEX		= 0,
		FRAGMENT	= 1,
	};

	///
	/// @brief
	///
	struct ShaderSources
	{
		std::string Vertex;		///< Vertex Shader source code
		std::string Fragment;	///< Fragment Shader source code
	};


	class ShaderManager;
	class ShaderLoader;
}
