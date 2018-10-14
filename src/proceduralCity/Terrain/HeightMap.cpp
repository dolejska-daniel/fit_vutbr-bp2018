///
/// @file HeightMap.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/gtc/noise.hpp> // perlin

#include "HeightMap.h"
#include "Chunk.h"


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

float Terrain::HeightMap::GetData(float x, float y, unsigned int detail)
{
	//	TODO: Octaves
	//	TODO: Vars
	float scale = 16.f;
	float amplitude = 4;
	float frequency = 1;

	//	TODO: Octaves
	//	TODO: Vars
	float persistence = 1;
	float lacunarity = 1;


	srand(123456); //	TODO: vars: terrain.seed
	int offsetX = rand() % 200000 - 100000;
	int offsetY = rand() % 200000 - 100000;

	glm::vec2 sample(0);
	sample.y = y / scale * frequency + offsetY;
	sample.x = x / scale * frequency + offsetX;

	//	Posunutí intervalu do <-1, 1>
	float h = glm::perlin(sample) * 2 - 1;
	h *= amplitude;

	//printf("perlin[%2d, %2d] = (%f, %f, %f)\n", x, y, sample.x, sample.y, h);
	return h;
}
