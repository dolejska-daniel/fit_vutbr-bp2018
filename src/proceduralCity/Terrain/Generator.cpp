///
/// @file Generator.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Terrain/Map.h>
#include <Terrain/Chunk.h>
#include <Terrain/Builder.h>
#include <Terrain/Generator.h>


using namespace Terrain;

Map* Generator::GenerateMap(vars::Vars& vars)
{
	return new Map(vars);
}

Chunk* Generator::GenerateChunk(Map* map, const int globalOffsetX, const int globalOffsetY)
{
	assert(map != nullptr);

	//	Vytvoření instance chunku
	//	- vygenerování výškové mapy a inicializace objektu proběhne automaticky
	const auto chunk = new Chunk(map->GetVars(), globalOffsetX, globalOffsetY);

	//	Sestavení vertexů a indexů chunku
	Builder::BuildVertices(chunk, map->GetHeightMap());
	Builder::BuildIndices(chunk);

	return chunk;
}
