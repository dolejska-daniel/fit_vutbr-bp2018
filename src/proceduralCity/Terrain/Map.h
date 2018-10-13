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
	class Map
	{
		friend Chunk;

	public:
		Map(vars::Vars& vars);
		~Map();

		Chunk** chunks;

	private:
		vars::Vars& vars;

	};
}
