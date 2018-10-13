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
	namespace Builder
	{
		void BuildVertices(Chunk* chunk);
		void BuildIndices(Chunk* chunk);
	}
}
