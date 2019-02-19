///
/// @file HeightMap.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/gtc/noise.hpp> // perlin
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp> // lerp
#include <Terrain/Chunk.h>
#include <Terrain/HeightMap.h>


using namespace Terrain;

HeightMap::HeightMap(vars::Vars& vars)
	: _vars(vars)
{
	const auto octaveCount = _vars.getUint32("terrain.octaves");
	_offsets = new glm::vec2[octaveCount];
	for (unsigned int i = 0; i < octaveCount; i++)
	{
		_offsets[i].x = float(rand() % 200000 - 100000);
		_offsets[i].y = float(rand() % 200000 - 100000);
	}

	minNoise = maxNoise = GenerateNoise(0.f, 0.f);
	for (auto x = 0; x < 512; x++)
	{
		const auto noise = GenerateNoise(x, 0.f);
		maxNoise = glm::max(noise, maxNoise);
		minNoise = glm::min(noise, minNoise);
	}

	std::cerr << "ALL SET!" << std::endl;
	std::cerr << minNoise << std::endl;
	std::cerr << maxNoise << std::endl;
	std::cerr << std::endl << std::endl << std::endl;
}

HeightMap::~HeightMap()
{
	delete[] _offsets;
}

float HeightMap::GenerateNoise(float globalX, float globalY) const
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

	float result = 0;
	glm::vec2 sample(0);
	for (unsigned int i = 0; i < octaveCount; i++)
	{
		sample.y = globalY / scale * frequency + _offsets[i].x;
		sample.x = globalX / scale * frequency + _offsets[i].y;

		//	Posunutí intervalu do <-1, 1>
		float h = glm::perlin(sample) * 2 - 1;
		result += h * amplitude;

		amplitude *= persistence;
		frequency *= lacunarity;
	}

	return result;
}


float HeightMap::GetData(const float globalX, const float globalY, unsigned int detail) const
{
	auto noise = GenerateNoise(globalX, globalY);

	//std::cerr << result << std::endl;
	//std::cerr << "Aft: " << result << std::endl;
	PreprocessData(noise);
	//printf("perlin[%2d, %2d] = (%f, %f, %f)\n", globalX, globalY, sample.globalX, sample.globalY, h);
	return noise;
}

float HeightMap::GetData(glm::vec2 const& v, const unsigned detail) const
{
	return GetData(v.x, v.y, detail);
}

float HeightMap::GetData(glm::vec3 const& v, const unsigned detail) const
{
	return GetData(v.x, v.z, detail);
}

float HeightMap::GetData(glm::vec3 const& v) const
{
	return GetData(v, _vars.getUint32("terrain.detail"));
}

void HeightMap::PreprocessData(float& sample) const
{
	const float s = ilerp(minNoise, maxNoise, sample);

	if (s < .1f)
		sample = -40 + -20 * ilerp(.1f, -0.2f, s);
	else if (s < .15f)
		sample = -40 * ilerp(.15f, .1f, s);
	else if (s < 0.9f)
		sample = 5.f * ilerp(.2f, .9f, s);
	else if (s < 0.95f)
		sample = 5 + 10 * ilerp(.9f, .95f, s);
	else if (s < 0.98f)
		sample = 5 + 10 + 20 * ilerp(.95f, .98f, s);
	else if (s < 1.f)
		sample = 5 + 10 + 20 + 30 * ilerp(.98f, 1.f, s);
	else
		sample = 5 + 10 + 20 + 30 + 80 * ilerp(1.f, 1.1f, s);
}

float HeightMap::ilerp(float min, float max, float x) const
{
	return (x - min) / (max - min);
}
