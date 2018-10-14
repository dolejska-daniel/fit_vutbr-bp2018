///
/// @file Map.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

#include <Vars/Vars.h>

#include "Fwd.h"


namespace Terrain
{
	///
	/// @class Map
	/// @brief
	///
	class Map
	{
	public:
		vars::Vars& vars;
		Chunk** chunks;

		Map(vars::Vars& vars);
		~Map();

		inline HeightMap* GetHeightMap() { return heightMap; }

	private:
		HeightMap* heightMap;
	};
}
