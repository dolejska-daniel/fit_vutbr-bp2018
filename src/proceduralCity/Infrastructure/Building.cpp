///
/// @file Building.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <algorithm>
#include <Infrastructure/Parcel.h>
#include <Infrastructure/Building.h>
#include <Infrastructure/BuildingPart.h>
#include <Utils/functions.h>

using namespace Infrastructure;


Building::Building(std::shared_ptr<Parcel> parcel, Terrain::HeightMap *heightMap, const BuildingType type)
	: _parcel(std::move(parcel)), _type(type), _heightMap(heightMap)
{
	GenerateParts();
}

Building::~Building()
= default;

void Building::GenerateParts()
{
	if (_type == SQUARE)
		GenerateParts_Square();
	else if (_type == STREET_SQUARE)
		GenerateParts_StreetSquare();
}

void Building::GenerateParts_Square()
{
	const auto points = _parcel->GetBorderPoints();
	auto max = points.front(), min = points.front();
	for (auto& point : points)
	{
		max.x = std::max(max.x, point.x);
		max.z = std::max(max.z, point.z);
		min.x = std::min(min.x, point.x);
		min.z = std::min(min.z, point.z);
	}

	Utils::RectBounds bounds(min, max);
	/*
	auto a = Utils::vec2to3(bounds.LeftBottom());
	auto b = Utils::vec2to3(bounds.LeftTop());
	auto c = Utils::vec2to3(bounds.RightBottom());
	*/
	auto a = points[0];
	auto b = points[1];
	auto c = points[3];

	auto u = b - a;
	auto v = c - a;

	auto dot = glm::abs(glm::dot(u, v));
	for (auto i = 0; i < points.size(); ++i)
	{
		auto i1 = (i + 1) % points.size();
		auto i2 = i - 1;
		if (i2 < 0) i2 += points.size();

		auto a_ = points[i];
		auto b_ = points[i1];
		auto c_ = points[i2];

		auto u_ = b_ - a_;
		auto v_ = c_ - a_;

		auto dot_ = glm::abs(glm::dot(u_, v_));
		if (dot_ < dot)
		{
			a = a_;
			b = b_;
			c = c_;

			u = u_;
			v = v_;

			dot = dot_;
		}
	}
	std::cerr << dot << std::endl;

	auto u_len = glm::length(u);
	auto v_len = glm::length(v);

	std::cerr << u_len << std::endl;
	std::cerr << v_len << std::endl;
	auto len_ratio = glm::min(u_len, v_len);
	if (len_ratio < 10.f)
	{
		std::cerr << "Not building on parcel, len is too low: " << len_ratio << std::endl;
		return;
	}

	auto density = 2.f;
	float x_max, y_max;
	glm::vec3 z_offset, x_offset;

	if (v_len > u_len)
	{
		x_max = density;
		y_max = std::floor(v_len / (u_len / density));
	}
	else
	{
		x_max = std::floor(u_len / (v_len / density));
		y_max = density;
	}

	auto ratio = glm::max(x_max / y_max, y_max / x_max);
	if (ratio > 5.f)
	{
		std::cerr << "Not building on parcel, ratio is too big: " << ratio << std::endl;
		return;
	}

	x_offset = u / x_max;
	z_offset = v / y_max;

	// TODO: Minimální rozměry parcely?

	std::vector<glm::vec3> part_points;
	part_points.reserve(4);
	for (auto x = 0; x < x_max; x++)
	{
		for (auto y = 0; y < y_max; y++)
		{
			auto z_merge = 1.f, x_merge = 1.f;

			auto offset = a + float(x) * x_offset + float(y) * z_offset;
			offset.y = 10.f;

			if (y < y_max - 1 && rand() % 10 <= 2)
			{
				z_merge = 2.f;
				y += 1;
			}
			part_points.push_back(offset);
			part_points.push_back(offset + z_merge * z_offset);
			part_points.push_back(offset + z_merge * z_offset + x_offset);
			part_points.push_back(offset + x_offset);

			const auto part = std::make_shared<BuildingPart>(_heightMap, part_points, _type);
			parts.push_back(part);

			part_points.clear();
		}
	}
}

void Building::GenerateParts_StreetSquare()
{
	const auto part = std::make_shared<BuildingPart>(_heightMap, _parcel->GetBorderPoints(), STREET_SQUARE);
	parts.push_back(part);
}
