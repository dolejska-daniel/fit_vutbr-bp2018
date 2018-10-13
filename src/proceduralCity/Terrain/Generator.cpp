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

Terrain::Chunk* Terrain::Generator::GenerateChunk(Terrain::Map* map)
{
	assert(map != nullptr);

	//	Vytvoøení instance chunku
	//	automaticky probìhne vygenerování vıškové mapy a inicializace objektu
	Chunk* chunk = new Chunk(map);

	//	Sestavení vertexù a indexù chunku
	Terrain::Builder::BuildVertices(chunk);
	Terrain::Builder::BuildIndices(chunk);

	return chunk;
}
