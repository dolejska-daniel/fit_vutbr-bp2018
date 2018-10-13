///
/// @file Chunk.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include "Chunk.h"
#include "Map.h"
#include "HeightMap.h"


Terrain::Chunk::Chunk(Terrain::Map* map)
{
	heightMap = new HeightMap(map->vars, this);
}

Terrain::Chunk::~Chunk()
{
}
