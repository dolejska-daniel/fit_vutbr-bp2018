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


Terrain::Map* Terrain::Generator::GenerateMap(vars::Vars& vars)
{
	return new Map(vars);
}

Terrain::Chunk* Terrain::Generator::GenerateChunk(Terrain::Map* map, int globalOffsetX, int globalOffsetY)
{
	assert(map != nullptr);

	//	Vytvoření instance chunku
	//	- vygenerování výškové mapy a inicializace objektu proběhne automaticky
	Chunk* chunk = new Chunk(map->vars, globalOffsetX, globalOffsetY);

	//	Sestavení vertexů a indexů chunku
	Terrain::Builder::BuildVertices(chunk, map->GetHeightMap());
	Terrain::Builder::BuildIndices(chunk);

	return chunk;
}
