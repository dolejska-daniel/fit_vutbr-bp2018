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
#include <Infrastructure/Street.h>


using namespace Terrain;

Map::Map(vars::Vars& vars)
	: _vars(vars), _width(vars.getUint32("terrain.map.width")), _height(vars.getUint32("terrain.map.height")), _chunkWidth(vars.getUint32("terrain.chunk.width")), _chunkHeight(vars.getUint32("terrain.chunk.height"))
{
	const auto halfWidth = _width / 2;
	const auto halfHeight = _height / 2;

	_heightMap = new HeightMap(vars);

	const auto GenerateTopRow = [&](const int index) {
		const int initialX = -index + 1;
		const int y = index - 1;
		const int count = 2 * index - 1;
		int chunkIndexOffset = index - 2;
		chunkIndexOffset = 4 * chunkIndexOffset * chunkIndexOffset + 4 * chunkIndexOffset + 1; // 4x^2 + 4x + 1
		for (int i = 0; i < count; i++)
		{
			GetChunk(chunkIndexOffset + i) = Generator::GenerateChunk(this, GetChunkOffsetX(initialX + i), GetChunkOffsetY(y));
		}
	};

	const auto GenerateRightColumn = [&](const int index) {
		const int x = index - 1;
		const int initialY = index - 2;
		const int count = 2 * index - 3;
		int chunkIndexOffset = index - 2;
		chunkIndexOffset = 4 * chunkIndexOffset * chunkIndexOffset + 4 * chunkIndexOffset + 1; // 4x^2 + 4x + 1
		chunkIndexOffset += 2 * index - 1; // Top row
		for (int i = 0; i < count; i++)
		{
			GetChunk(chunkIndexOffset + i) = Generator::GenerateChunk(this, GetChunkOffsetX(x), GetChunkOffsetY(initialY + i));
		}
	};

	const auto GenerateLeftColumn = [&](const int index) {
		const int x = -index + 1;
		const int initialY = index - 2;
		const int count = 2 * index - 3;
		int chunkIndexOffset = index - 2;
		chunkIndexOffset = 4 * chunkIndexOffset * chunkIndexOffset + 4 * chunkIndexOffset + 1; // 4x^2 + 4x + 1
		chunkIndexOffset += 2 * index - 1; // Top row
		chunkIndexOffset += 2 * index - 3; // Right column
		chunkIndexOffset += 2 * index - 1; // Bottom row
		for (int i = 0; i < count; i++)
		{
			GetChunk(chunkIndexOffset + i) = Generator::GenerateChunk(this, GetChunkOffsetX(x), GetChunkOffsetY(initialY + i));
		}
	};

	const auto GenerateBottomRow = [&](const int index) {
		const int initialX = -index + 1;
		const int y = -index + 1;
		const int count = 2 * index - 1;
		int chunkIndexOffset = index - 2;
		chunkIndexOffset = 4 * chunkIndexOffset * chunkIndexOffset + 4 * chunkIndexOffset + 1; // 4x^2 + 4x + 1
		chunkIndexOffset += 2 * index - 1; // Top row
		chunkIndexOffset += 2 * index - 3; // Right column
		for (int i = 0; i < count; i++)
		{
			GetChunk(chunkIndexOffset + i) = Generator::GenerateChunk(this, GetChunkOffsetX(initialX + i), GetChunkOffsetY(y));
		}
	};

	GetChunk(0) = Generator::GenerateChunk(this, GetChunkOffsetX(0), GetChunkOffsetY(0));
	for (unsigned i = 2; i <= _width; i++)
	{
		GenerateTopRow(i);
		GenerateRightColumn(i);
		GenerateBottomRow(i);
		GenerateLeftColumn(i);
	}
}

Map::~Map()
= default;


std::shared_ptr<Chunk>& Map::GetChunk(const unsigned int index)
{
	return _chunks[index];
}

void Map::ValidateStreet(std::shared_ptr<Infrastructure::Street> const& street)
{
	/*
	auto v = street->GetSegment().endPoint;
	const auto x = GetChunkOffsetX(_width);
	const auto y = GetChunkOffsetY(_width);
	std::cerr << v.x << ", " << v.y << ", " << v.z << std::endl;
	std::cerr << x << std::endl;
	std::cerr << y << std::endl;
	if (v.x < x
		|| v.x > x + _chunkWidth
		|| v.z < y
		|| v.z > y + _chunkWidth)
	{
		_width++;
		std::cerr << "Extending map!" << std::endl;
	}*/
}
