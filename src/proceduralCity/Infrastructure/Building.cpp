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
#include "Utils/functions.h"
#include <glm/detail/func_geometric.inl>

using namespace Infrastructure;


Building::Building(std::shared_ptr<Parcel> parcel, const BuildingType type)
	: _parcel(std::move(parcel)), _type(type)
{
	GenerateParts();
}

Building::~Building()
= default;

void Building::GenerateParts()
{
	if (_type == SQUARE)
		GenerateParts_Square();
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
	auto ab = b - a;
	auto ab_len = glm::length(ab);
	auto ac = c - a;
	auto ac_len = glm::length(ac);

	auto density = 3.f;
	float x_max, y_max;
	glm::vec3 z_offset, x_offset;

	if (ac_len > ab_len)
	{
		x_max = density;
		y_max = std::floor(ac_len / (ab_len / density));
	}
	else
	{
		x_max = std::floor(ab_len / (ac_len / density));
		y_max = density;
	}

	x_offset = ab / x_max;
	z_offset = ac / y_max;

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

			const auto part = std::make_shared<BuildingPart>(part_points, _type);
			parts.push_back(part);

			part_points.clear();
		}
	}
}
