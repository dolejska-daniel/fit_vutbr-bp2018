///
/// @file Builder.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Terrain/Fwd.h>


namespace Terrain
{
	///
	/// @brief
	///
	namespace Builder
	{
		///
		/// @brief
		///
		void BuildVertices(Chunk* chunk, HeightMap* heightMap);
		///
		/// @brief
		///
		void BuildIndices(Chunk* chunk);
	}
}
