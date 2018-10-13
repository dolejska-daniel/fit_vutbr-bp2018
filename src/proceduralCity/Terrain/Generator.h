///
/// @file Generator.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

#include <Vars/Vars.h>

#include "Fwd.h"


namespace Terrain
{
	namespace Generator
	{
		Map* GenerateMap(vars::Vars& vars);
		Chunk* GenerateChunk(Terrain::Map* map);
	};
}
