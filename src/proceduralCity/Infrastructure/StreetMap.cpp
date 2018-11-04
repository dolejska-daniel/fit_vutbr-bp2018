///
/// @file StreetMap.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/mat2x2.hpp>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetMap.h>


using namespace Infrastructure;

float stepSize = 10.f;
float stepLevelOffset = .5f;


StreetMap::StreetMap()
{
	auto street = std::make_shared<Street>(glm::vec3(0, 4, 0), glm::vec3(1, 0, 0), stepSize / 2);
	GetStreets().push_back(street);

	street = std::make_shared<Street>(glm::vec3(0, 4, 0), glm::vec3(-.5, 0, -.5), stepSize / 2);
	GetStreets().push_back(street);

	street = std::make_shared<Street>(glm::vec3(0, 4, 0), glm::vec3(-.5, 0, .5), stepSize / 2);
	GetStreets().push_back(street);
}

StreetMap::~StreetMap()
= default;


StreetSegmentIntersection StreetMap::Intersection(StreetSegment const& segment, std::shared_ptr<Street> const& street)
{
	StreetSegmentIntersection intersection = {
		false,
		glm::vec3(0),
	};

	auto streetSegments = street->GetSegments();
	for (auto streetSegment : streetSegments)
	{
		if (segment.direction == streetSegment.direction)
		{
			//	Segmenty jsou rovnoběžné, kolize není možná
			continue;
		}

		//--------------------------------------------------------------------
		//	Tomáš Milet
		//
		auto c = segment.endPoint - segment.startPoint;
		auto p = streetSegment.startPoint - streetSegment.endPoint;
		auto h = segment.startPoint - streetSegment.startPoint;

		const auto r = h[2] * p[0] - h[0] * p[2];
		const auto x = c[2] * h[0] - c[0] * h[2];
		const auto s = c[2] * p[0] - c[0] * p[2];

		auto t = intersection.positionRelative = { -r / s, -x / s };
		//intersection.exists = t[0] >= 0 && t[0] <= 1 && t[1] >= 0 && t[1] <= 1;
		intersection.exists = t[0] > 0.f && t[0] < 1.f && t[1] > 0.f && t[1] < 1.f;
		return intersection;
		//--------------------------------------------------------------------
	}

	return intersection;
}

std::vector<StreetSegmentIntersection> StreetMap::IntersectionsWithAny(StreetSegment const& segment) const
{
	std::vector<StreetSegmentIntersection> intersections = {};
	//	Kontrola lokálních kolizí (tento StreetMap)
	//for (size_t i = ReadStreets().size() - 1; i >= 0 && i != std::numeric_limits<size_t>::max(); i--)
	for (size_t i = 0; i < ReadStreets().size(); i++)
	{
		//	Načtení ulice pro ověření
		const auto street = ReadStreets()[i];
		//	TODO: Vybírat pouze relevantní ulice (nacházející se v blízkosti segmentu)

		//	Kontrola průsečíku
		auto intersection = Intersection(segment, street);
		if (intersection.exists)
		{
			//	Průsečík existuje
			intersections.push_back(intersection);
		}
	}
	//	TODO: Kontrolovat na globální úrovni (i v rámci ostatních StreetMap)

	return intersections;
}

void StreetMap::BuildStep()
{
	for (size_t i = 0; i < GetStreets().size(); i++)
	{
		auto street = GetStreets()[i];
		if (street->Ended())
			continue;

		float length = glm::pow(stepLevelOffset, street->GetLevel()) * stepSize;
		street->BuildStep(length);

		auto segment = street->GetSegment();
		auto intersections = IntersectionsWithAny(segment);
		if (!intersections.empty())
		{
			for (auto intersection : intersections)
			{
				auto endPoint = street->GetSegmentPoint(intersection.positionRelative[0] - 0.001f);
				street->SetSegmentEndPoint(endPoint);
			}

			street->End();
			continue;
		}

		const int split = glm::floor(segment.length / length);
		if (split > segment.lastSplit && street->GetLevel() < 3)
		{
			segment.lastSplit = split;

			glm::vec3 position = street->GetSegmentPoint(1.f);
			glm::vec3 direction;

			if (std::rand() % 10 <= 5)
			{
				direction.x = -segment.direction.z;
				direction.y = segment.direction.y;
				direction.z = segment.direction.x;
			}
			else
			{
				direction.x = segment.direction.z;
				direction.y = segment.direction.y;
				direction.z = -segment.direction.x;
			}

			auto newStreet = std::make_shared<Street>(position, direction, length, street->GetLevel() + 1);
			GetStreets().push_back(newStreet);
		}
	}
}
