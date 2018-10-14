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
	enum ShaderType
	{
		NONE		= -1,
		VERTEX		= 0,
		GEOMETRY	= 1,
		FRAGMENT	= 2,
	};

	///
	/// @brief
	///
	struct ShaderSources
	{
		std::string Vertex;		///< Vertex Shader source code
		std::string Geometry;	///< Geometry Shader source code
		std::string Fragment;	///< Fragment Shader source code
	};


	class ShaderManager;
	class ShaderLoader;
}
