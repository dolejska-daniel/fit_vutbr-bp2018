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
#include <Application/Application.h>


using namespace Terrain;

Map::Map()
	: _width(Application::Vars.getUint32("terrain.map.width")),
	_height(Application::Vars.getUint32("terrain.map.height")),
	_chunkWidth(Application::Vars.getUint32("terrain.chunk.width")),
	_chunkHeight(Application::Vars.getUint32("terrain.chunk.height"))
{
	const int halfWidth = _width / 2;
	const int halfHeight = _height / 2;

	_heightMap = new HeightMap();

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

	/*
	GetChunk(0) = Generator::GenerateChunk(this, GetChunkOffsetX(0), GetChunkOffsetY(0));
	for (unsigned i = 2; i <= _width; i++)
	{
		GenerateTopRow(i);
		GenerateRightColumn(i);
		GenerateBottomRow(i);
		GenerateLeftColumn(i);
	}
	*/


	for (int y = 0; y < _height; ++y)
	{
		for (int x = 0; x < _width; ++x)
		{
			GetChunk(y * _width + x) = Generator::GenerateChunk(this, GetChunkOffsetX(-halfWidth + x), GetChunkOffsetY(-halfHeight + y));
		}
	}

	/*
	GetChunk(0) = Generator::GenerateChunk(this, GetChunkOffsetX(0), GetChunkOffsetY(0));
	for (unsigned i = 2; i <= _width; i++)
	{
		GenerateTopRow(i);
		GenerateRightColumn(i);
		GenerateBottomRow(i);
		GenerateLeftColumn(i);
	}*/
}

Map::~Map()
= default;


std::shared_ptr<Chunk>& Map::GetChunk(const unsigned int index)
{
	return _chunks[index];
}

bool Map::ValidateStreet(std::shared_ptr<Infrastructure::Street> const& street) const
{
	const auto w = _width - 1;
	const auto v = street->GetSegment().endPoint;
	//auto d = Application::Vars.getUint32("terrain.chunk.width") * Application::Vars.getFloat("terrain.chunk.scale");
	const auto d = 0;

	const glm::vec2 max{ GetChunkOffsetX(w + 1) + d, GetChunkOffsetY(w + 1) + d };
	const glm::vec2 min{ GetChunkOffsetX(-w), GetChunkOffsetY(-w) };
	if (v.x < min.x || v.z < min.y || v.x > max.x || v.z > max.y)
	{
		street->End();
		return false;
	}

	return true;
}
