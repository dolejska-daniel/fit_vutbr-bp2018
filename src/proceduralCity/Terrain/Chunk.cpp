///
/// @file Chunk.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Vars/Vars.h>
#include <Terrain/Map.h>
#include <Terrain/Chunk.h>


using namespace Terrain;

Chunk::Chunk(vars::Vars& vars, const int globalOffsetX, const int globalOffsetY)
	: _globalOffsetX(globalOffsetX), _globalOffsetY(globalOffsetY)
{
	_detail = vars.getUint32("terrain.detail");
	_width  = vars.getUint32("terrain.chunk.width");
	_height = vars.getUint32("terrain.chunk.height");
	_scale  = vars.getFloat("terrain.chunk.scale");
}

Chunk::~Chunk()
{
	delete[] _vertices;
	delete[] _indices;
}

unsigned Chunk::GetIndicesWidth() const
{
	return GetWidth() * GetDetail();
}

unsigned Chunk::GetIndicesHeight() const
{
	return GetHeight() * GetDetail();
}

unsigned Chunk::GetIndicesSize() const
{
	return GetIndicesWidth() * GetIndicesHeight() * sizeof(ChunkIndex);
}

unsigned Chunk::GetVerticesWidth() const
{
	return (GetWidth() + 1) * GetDetail();
}

unsigned Chunk::GetVerticesHeight() const
{
	return (GetHeight() + 1) * GetDetail();
}

unsigned Chunk::GetVerticesSize() const
{
	return GetVerticesWidth() * GetVerticesHeight() * sizeof(ChunkVertex);
}
