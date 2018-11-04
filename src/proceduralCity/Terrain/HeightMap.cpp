///
/// @file HeightMap.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/gtc/noise.hpp> // perlin
#include <Terrain/Chunk.h>
#include <Terrain/HeightMap.h>


using namespace Terrain;

HeightMap::HeightMap(vars::Vars& vars)
	: _vars(vars)
{
	//	TODO: vars:
	//	- terrain.seed
	//	- terrain.scale
	//	- ?terrain._detail
	//	- terrain.chunk._width
	//	- terrain.chunk._height
}

HeightMap::~HeightMap()
= default;


float HeightMap::GetData(const float globalX, const float globalY, unsigned int detail) const
{
	//	TODO: Detail?

	//
	// https://github.com/SebLague/Procedural-Landmass-Generation
	//
	const auto scale = _vars.getFloat("terrain.scale");
	auto amplitude = _vars.getFloat("terrain.amplitude");
	auto frequency = _vars.getFloat("terrain.frequency");

	const auto persistence = _vars.getFloat("terrain.persistence");
	const auto lacunarity = _vars.getFloat("terrain.lacunarity");
	const auto octaveCount = _vars.getUint32("terrain.octaves");

	srand(_vars.getUint32("terrain.seed"));

	auto* offsets = new glm::vec2[octaveCount];
	for (unsigned int i = 0; i < octaveCount; i++)
	{
		offsets[i].x = float(rand() % 200000 - 100000);
		offsets[i].y = float(rand() % 200000 - 100000);
	}

	float result = 0;
	glm::vec2 sample(0);
	for (unsigned int i = 0; i < octaveCount; i++)
	{
		sample.y = globalY / scale * frequency + offsets[i].x;
		sample.x = globalX / scale * frequency + offsets[i].y;

		//	Posunutí intervalu do <-1, 1>
		const auto h = glm::perlin(sample) * 2 - 1;
		result += h * amplitude;

		amplitude *= persistence;
		frequency *= lacunarity;
	}

	//printf("perlin[%2d, %2d] = (%f, %f, %f)\n", globalX, globalY, sample.globalX, sample.globalY, h);
	return result;
}

float HeightMap::GetData(glm::vec2 const& v, const unsigned detail)
{
	return GetData(v.x, v.y, detail);
}

float HeightMap::GetData(glm::vec3 const& v, const unsigned detail)
{
	return GetData(v.x, v.z, detail);
}
