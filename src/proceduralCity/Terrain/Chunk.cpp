///
/// @file Chunk.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Vars/Vars.h>
#include <Terrain/Map.h>
#include <Terrain/Chunk.h>
#include <Terrain/HeightMap.h>


Terrain::Chunk::Chunk(vars::Vars& vars, int globalOffsetX, int globalOffsetY)
	: globalOffsetX(globalOffsetX), globalOffsetY(globalOffsetY)
{
	detail = vars.getUint32("terrain.detail");
	width  = vars.getUint32("terrain.chunk.width");
	height = vars.getUint32("terrain.chunk.height");
}

Terrain::Chunk::~Chunk()
{
	delete[] vertices;
	delete[] indices;
}
