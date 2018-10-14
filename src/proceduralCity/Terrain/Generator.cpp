///
/// @file Generator.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include "Generator.h"
#include "Builder.h"
#include "Chunk.h"
#include "Map.h"


Terrain::Map* Terrain::Generator::GenerateMap(vars::Vars& vars)
{
	return new Map(vars);
}

Terrain::Chunk* Terrain::Generator::GenerateChunk(Terrain::Map* map, int globalOffsetX, int globalOffsetY)
{
	assert(map != nullptr);

	//	Vytvoøení instance chunku
	//	- vygenerování výškové mapy a inicializace objektu probìhne automaticky
	Chunk* chunk = new Chunk(map->vars, globalOffsetX, globalOffsetY);

	//	Sestavení vertexù a indexù chunku
	Terrain::Builder::BuildVertices(chunk, map->GetHeightMap());
	Terrain::Builder::BuildIndices(chunk);

	return chunk;
}
