///
/// @file Generator.cpp
/// @brief
///
/// @author Daniel Dolej�ka <xdolej08@stud.fit.vutbr.cz>
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

	//	Vytvo�en� instance chunku
	//	automaticky prob�hne vygenerov�n� v��kov� mapy a inicializace objektu
	Chunk* chunk = new Chunk(map);

	//	Sestaven� vertex� a index� chunku
	Terrain::Builder::BuildVertices(chunk);
	Terrain::Builder::BuildIndices(chunk);

	return chunk;
}
