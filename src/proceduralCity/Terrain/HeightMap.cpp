///
/// @file HeightMap.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/gtc/noise.hpp> // perlin
#include <Terrain/Chunk.h>
#include <Terrain/HeightMap.h>


Terrain::HeightMap::HeightMap(vars::Vars& vars)
	: vars(vars)
{
	//	TODO: vars:
	//	- terrain.seed
	//	- terrain.scale
	//	- ?terrain.detail
	//	- terrain.chunk.width
	//	- terrain.chunk.height
}

Terrain::HeightMap::~HeightMap()
{

}

float Terrain::HeightMap::GetData(float globalX, float globalY, unsigned int detail)
{
	//
	// https://github.com/SebLague/Procedural-Landmass-Generation
	//

	float scale = vars.getFloat("terrain.scale");
	float amplitude = vars.getFloat("terrain.amplitude");
	float frequency = vars.getFloat("terrain.frequency");

	float persistence = vars.getFloat("terrain.persistence");
	float lacunarity = vars.getFloat("terrain.lacunarity");
	unsigned int octaveCount = vars.getUint32("terrain.octaves");

	srand(vars.getUint32("terrain.seed"));

	glm::vec2 *offsets = new glm::vec2[octaveCount];
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
		float h = glm::perlin(sample) * 2 - 1;
		result += h * amplitude;

		amplitude *= persistence;
		frequency *= lacunarity;
	}

	//printf("perlin[%2d, %2d] = (%f, %f, %f)\n", globalX, globalY, sample.globalX, sample.globalY, h);
	return result;
}
