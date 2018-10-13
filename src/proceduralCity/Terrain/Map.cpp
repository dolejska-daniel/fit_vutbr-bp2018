///
/// @file Map.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include "Map.h"
#include "Chunk.h"
#include "Generator.h"


Terrain::Map::Map(vars::Vars& vars)
	: vars(vars)
{
	chunks = new Chunk*[32];
	chunks[0] = Terrain::Generator::GenerateChunk(this);
}

Terrain::Map::~Map()
{
	//	TODO: Free individual chunks
	delete[] chunks;
}
