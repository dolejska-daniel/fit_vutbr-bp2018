///
/// @file Map.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Terrain/Map.h>
#include <Terrain/Chunk.h>
#include <Terrain/HeightMap.h>
#include <Terrain/Generator.h>


using namespace Terrain;

Map::Map(vars::Vars& vars)
	: _vars(vars), _width(vars.getUint32("terrain.map.width")), _height(vars.getUint32("terrain.map.height")), _chunkWidth(vars.getUint32("terrain.chunk.width")), _chunkHeight(vars.getUint32("terrain.chunk.height"))
{
	const auto halfWidth = _width / 2;
	const auto halfHeight = _height / 2;

	const auto globalX = -(static_cast<int>(_chunkWidth) / 2);
	const auto globalY = -(static_cast<int>(_chunkHeight) / 2);

	_heightMap = new HeightMap(vars);

	const auto GetChunkOffsetX = [&](const int x) -> int {
		return globalX + (x - halfWidth) * _chunkWidth;
	};

	const auto GetChunkOffsetY = [&](const int y) -> int {
		return globalX + (y - halfHeight) * _chunkHeight;
	};

	for (unsigned int y = 0; y < _height; y++)
	{
		for (unsigned int x = 0; x < _width; x++)
		{
			GetChunk(x, y) = Generator::GenerateChunk(this, GetChunkOffsetX(x), GetChunkOffsetY(y));
		}
	}
}

Map::~Map()
= default;


std::shared_ptr<Chunk>& Map::GetChunk(const unsigned int x, const unsigned int y)
{
	assert(x < _width);
	assert(y < _height);
	return _chunks[y * _width + x];
}

std::shared_ptr<Chunk> Map::ReadChunk(const unsigned int x, const unsigned int y) const
{
	assert(x < _width);
	assert(y < _height);
	return _chunks.at(y * _width + x);
}
