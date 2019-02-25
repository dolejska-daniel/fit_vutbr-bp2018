///
/// @file HeightMap.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/gtc/noise.hpp> // perlin
#include <Terrain/Chunk.h>
#include <Terrain/HeightMap.h>
#include <Utils/Curve.h>


using namespace Terrain;

HeightMap::HeightMap(vars::Vars& vars)
	: _vars(vars), minNoise(0), maxNoise(0)
{
	auto amplitude = _vars.getFloat("terrain.amplitude");
	auto frequency = _vars.getFloat("terrain.frequency");

	const auto octaveCount = _vars.getUint32("terrain.octaves");
	const auto persistence = _vars.getFloat("terrain.persistence");
	const auto lacunarity = _vars.getFloat("terrain.lacunarity");

	_offsets = new glm::vec2[octaveCount];
	for (unsigned int i = 0; i < octaveCount; i++)
	{
		_offsets[i].x = float(rand() % 200000 - 100000);
		_offsets[i].y = float(rand() % 200000 - 100000);
	}

	for (unsigned int i = 0; i < octaveCount; i++)
	{
		maxNoise +=  0.5f * amplitude;
		minNoise += -0.5f * amplitude;

		amplitude *= persistence;
		frequency *= lacunarity;
	}

	auto heightCurvePoints = std::make_shared<std::vector<glm::vec2>>();
	heightCurvePoints->push_back({ 0.f, -1.0f });
	heightCurvePoints->push_back({ 0.f, -2.0f });
	heightCurvePoints->push_back({ 0.f, -1.0f });
	heightCurvePoints->push_back({ 0.f, -2.0f });
	heightCurvePoints->push_back({ 0.f,  0.0f });
	heightCurvePoints->push_back({ 0.f,  0.0f });
	heightCurvePoints->push_back({ 0.f,  0.02f });
	heightCurvePoints->push_back({ 0.f,  0.04f });
	heightCurvePoints->push_back({ 0.f,  0.06f });
	heightCurvePoints->push_back({ 0.f,  0.08f });
	heightCurvePoints->push_back({ 0.f,  0.1f });
	heightCurvePoints->push_back({ 0.f,  0.1f });
	heightCurvePoints->push_back({ 0.f,  0.1f });
	heightCurvePoints->push_back({ 0.f,  0.1f });
	heightCurvePoints->push_back({ 0.f,  0.12f });
	heightCurvePoints->push_back({ 0.f,  0.14f });
	heightCurvePoints->push_back({ 0.f,  0.16f });
	heightCurvePoints->push_back({ 0.f,  0.18f });
	heightCurvePoints->push_back({ 0.f,  0.2f });
	heightCurvePoints->push_back({ 0.f,  0.2f });
	heightCurvePoints->push_back({ 0.f,  0.2f });
	heightCurvePoints->push_back({ 0.f,  0.2f });
	heightCurvePoints->push_back({ 0.f,  2.f });
	heightCurvePoints->push_back({ 0.f,  4.f });
	heightCurvePoints->push_back({ 0.f,  8.f });
	_curve = std::make_shared<Utils::Curve>(heightCurvePoints);

	std::cerr << "ALL SET!" << std::endl;
	std::cerr << minNoise << std::endl;
	std::cerr << maxNoise << std::endl << std::endl;
	std::cerr << "Height mults: " << std::endl;
	std::cerr << _curve->GetPoint(0.f).y << std::endl;
	std::cerr << _curve->GetPoint(0.1f).y << std::endl;
	std::cerr << _curve->GetPoint(0.2f).y << std::endl;
	std::cerr << _curve->GetPoint(0.3f).y << std::endl;
	std::cerr << _curve->GetPoint(0.4f).y << std::endl;
	std::cerr << _curve->GetPoint(0.5f).y << std::endl;
	std::cerr << _curve->GetPoint(0.6f).y << std::endl;
	std::cerr << _curve->GetPoint(0.7f).y << std::endl;
	std::cerr << _curve->GetPoint(0.7f).y << std::endl;
	std::cerr << _curve->GetPoint(0.9f).y << std::endl;
	std::cerr << _curve->GetPoint(1.f).y << std::endl << std::endl;
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

		float h = glm::perlin(sample);
		result += h * amplitude;

		amplitude *= persistence;
		frequency *= lacunarity;
	}

	return result;
}


float HeightMap::GetData(const float x, const float y, unsigned int detail) const
{
	auto noise = GenerateNoise(x, y);

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
	const float s = glm::clamp(ilerp(minNoise, maxNoise, sample), 0.f, 1.f);
	sample = 40 * _curve->GetPoint(s).y;
}

float HeightMap::ilerp(float min, float max, float x) const
{
	return (x - min) / (max - min);
}
