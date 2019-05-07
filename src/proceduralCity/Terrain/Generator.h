///
/// @file Generator.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Vars/Fwd.h>
#include <Terrain/Fwd.h>


namespace Terrain
{
	///
	/// @brief
	///
	namespace Generator
	{
		///
		/// @brief
		///
		Map* GenerateMap();
		///
		/// @brief
		///
		std::shared_ptr<Chunk> GenerateChunk(Map* map, int globalOffsetX, int globalOffsetY);
	};
}
