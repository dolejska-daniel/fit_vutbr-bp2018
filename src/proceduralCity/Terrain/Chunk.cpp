///
/// @file Chunk.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Vars/Vars.h>

#include "Chunk.h"
#include "Map.h"
#include "HeightMap.h"


Terrain::Chunk::Chunk(vars::Vars & vars, int globalOffsetX, int globalOffsetY)
	: globalOffsetX(globalOffsetX), globalOffsetY(globalOffsetY)
{
	width  = vars.getUint32("terrain.chunk.width");
	height = vars.getUint32("terrain.chunk.height");
}

Terrain::Chunk::Chunk(unsigned int width, unsigned int height, int globalOffsetX, int globalOffsetY)
	: width(width), height(height), globalOffsetX(globalOffsetX), globalOffsetY(globalOffsetY) {}

Terrain::Chunk::~Chunk()
{
	delete[] vertices;
	delete[] indices;
}
