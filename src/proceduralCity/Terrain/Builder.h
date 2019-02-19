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
		void BuildVertices(std::shared_ptr<Chunk> chunk, HeightMap* heightMap);
		///
		/// @brief
		///
		void BuildIndices(std::shared_ptr<Chunk> const& chunk);
	}
}
