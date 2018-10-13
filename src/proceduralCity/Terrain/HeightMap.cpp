///
/// @file HeightMap.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/gtc/noise.hpp> // perlin

#include "HeightMap.h"
#include "Chunk.h"


Terrain::HeightMap::HeightMap(vars::Vars& vars, Terrain::Chunk* chunk)
	: vars(vars), chunk(chunk)/*, width(chunk), height(height)*/
{
	//	TODO: vars:
	//	- terrain.seed
	//	- terrain.scale
	//	- ?terrain.detail
	//	- terrain.chunk.width
	//	- terrain.chunk.height

	//	Alokace pamìti pro data
	map = new float[GetMapElementCount()];
	//	Generování dat
	Populate();
}

Terrain::HeightMap::~HeightMap()
{
	delete[] map;
}

void Terrain::HeightMap::Populate()
{
	//	TODO: Octaves
	//	TODO: Vars
	float scale = 8.f;
	float amplitude = 4;
	float frequency = 1;

	//	TODO: Octaves
	//	TODO: Vars
	float persistence = 1;
	float lacunarity = 1;


	srand(123456); //	TODO: vars: terrain.seed
	//int offsetX = rand() % 200000; //	TODO: plus offset pro jednotlivé terrain chunky
	//int offsetY = rand() % 200000;

	int offsetX = 0;
	int offsetY = 0;

	glm::vec3 v(0);
	for (int y = 0; y < GetHeight(); y++)
	{
		v.y = y / scale * frequency + offsetY;
		for (int x = 0; x < GetWidth(); x++)
		{
			v.x = x / scale * frequency + offsetX;

			//	Posunutí intervalu do <-1, 1>
			float h = glm::perlin(v)/* * 2 - 1*/;
			h *= amplitude;

			printf("perlin[%2d, %2d] = %f\n", x, y, h);
			map[GetMapIndex(x, y)] = h;
		}
	}
}

float Terrain::HeightMap::GetData(int x, int y)
{
	return map[GetMapIndex(x, y)];
}

unsigned int Terrain::HeightMap::GetWidth()
{
	assert(chunk != nullptr);
	return chunk->GetVerticesWidth();
}

unsigned int Terrain::HeightMap::GetHeight()
{
	assert(chunk != nullptr);
	return chunk->GetVerticesHeight();
}
