///
/// @file Application.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <string>
#include <geGL/Program.h>


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
		NONE		= -1,	///<
		VERTEX		= 0,	///< 
		GEOMETRY	= 1,	///<
		FRAGMENT	= 2,	///<
	};

	///
	/// @brief
	///
	struct ShaderSources
	{
		std::string Vertex;		///< Zdrojový kód pro Vertex Shader
		std::string Geometry;	///< Zdrojový kód pro Geometry Shader
		std::string Fragment;	///< Zdrojový kód pro Fragment Shader
	};
	
}
