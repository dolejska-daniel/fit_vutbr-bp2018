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
	: _vars(vars)
{
	const auto width = vars.getUint32("terrain.map.width");
	const auto halfWidth = width / 2;
	const auto height = vars.getUint32("terrain.map.height");
	const auto halfHeight = height / 2;

	const auto chunkWidth = vars.getUint32("terrain.chunk.width");
	const auto chunkHeight = vars.getUint32("terrain.chunk.height");

	const auto globalX = -(static_cast<int>(chunkWidth) / 2);
	const auto globalY = -(static_cast<int>(chunkHeight) / 2);

	_heightMap = new HeightMap(vars);
	_chunks = new Chunk*[width * height];

	const auto GetChunk = [&](const int x, const int y) -> Chunk*& {
		return _chunks[y * width + x];
	};

	const auto GetChunkOffsetX = [&](const int x) -> int {
		return globalX + (x - halfWidth) * chunkWidth;
	};

	const auto GetChunkOffsetY = [&](const int y) -> int {
		return globalX + (y - halfHeight) * chunkHeight;
	};

	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			GetChunk(x, y) = Generator::GenerateChunk(this, GetChunkOffsetX(x), GetChunkOffsetY(y));
		}
	}
}

Map::~Map()
{
	//	TODO: Free individual _chunks
	delete[] _chunks;
}
