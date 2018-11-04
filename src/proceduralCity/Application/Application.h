///
/// @file Application.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <string>
#include <map>
#include <SDL2/SDL.h>


///
/// @brief
///
namespace Application
{
	///
	/// @brief
	///
	extern std::map<SDL_Keycode, bool> KeyDown;

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
		std::string vertex;		///< Zdrojový kód pro Vertex Shader
		std::string geometry;	///< Zdrojový kód pro Geometry Shader
		std::string fragment;	///< Zdrojový kód pro Fragment Shader
	};
}
