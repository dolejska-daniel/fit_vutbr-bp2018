///
/// @file Builder.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

#include "Fwd.h"


namespace Terrain
{
	///
	/// @namespace Builder
	/// @brief
	///
	namespace Builder
	{
		void BuildVertices(Chunk* chunk, HeightMap* heightMap);
		void BuildIndices(Chunk* chunk);
	}
}
