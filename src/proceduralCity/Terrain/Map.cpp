///
/// @file Map.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include "Map.h"
#include "Chunk.h"
#include "HeightMap.h"
#include "Generator.h"


Terrain::Map::Map(vars::Vars& vars)
	: vars(vars)
{
	int width = vars.getUint32("terrain.map.width");
	int halfWidth = width / 2;
	int height = vars.getUint32("terrain.map.height");
	int halfHeight = height / 2;

	int chunkWidth = vars.getUint32("terrain.chunk.width");
	int chunkHeight = vars.getUint32("terrain.chunk.height");

	int globalX = -(chunkWidth / 2);
	int globalY = -(chunkHeight / 2);

	heightMap = new HeightMap(vars);
	chunks = new Chunk*[width * height];

	auto GetChunk = [&](int x, int y) -> Chunk*& {
		return chunks[y * width + x];
	};

	auto GetChunkOffsetX = [&](int x) -> int {
		return globalX + (x - halfWidth) * chunkWidth;
	};

	auto GetChunkOffsetY = [&](int y) -> int {
		return globalX + (y - halfHeight) * chunkHeight;
	};

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			GetChunk(x, y) = Terrain::Generator::GenerateChunk(this, GetChunkOffsetX(x), GetChunkOffsetY(y));
		}
	}
}

Terrain::Map::~Map()
{
	//	TODO: Free individual chunks
	delete[] chunks;
}
