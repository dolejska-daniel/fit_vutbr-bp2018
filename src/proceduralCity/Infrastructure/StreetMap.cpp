///
/// @file StreetMap.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/mat2x2.hpp>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetMap.h>


using namespace Infrastructure;

float stepSize = 10.f;
float stepLevelOffset = .5f;


StreetMap::StreetMap()
{
	auto street = std::make_shared<Infrastructure::Street>(glm::vec3(0, 4, 0), glm::vec3(1, 0, 0), stepSize / 2);
	GetStreets().push_back(street);

	street = std::make_shared<Infrastructure::Street>(glm::vec3(0, 4, 0), glm::vec3(-.5, 0, -.5), stepSize / 2);
	GetStreets().push_back(street);

	street = std::make_shared<Infrastructure::Street>(glm::vec3(0, 4, 0), glm::vec3(-.5, 0, .5), stepSize / 2);
	GetStreets().push_back(street);
}

StreetMap::~StreetMap()
{
}


StreetSegmentIntersection StreetMap::Intersection(const StreetSegment segment, const std::shared_ptr<Street> street) const
{
	StreetSegmentIntersection intersection = {
		false,
		glm::vec3(0),
	};

	auto streetSegments = street->GetSegments();
	for (size_t i = 0; i < streetSegments.size(); i++)
	{
		auto streetSegment = streetSegments[i];
		if (segment.direction == streetSegment.direction)
		{
			//	Segmenty jsou rovnoběžné, kolize není možná
			continue;
		}

		//--------------------------------------------------------------------
		//	Tomáš Milet
		//
		glm::vec3 C = segment.endPoint - segment.startPoint;
		glm::vec3 P = streetSegment.startPoint - streetSegment.endPoint;
		glm::vec3 H = segment.startPoint - streetSegment.startPoint;

		float R = H[2] * P[0] - H[0] * P[2];
		float X = C[2] * H[0] - C[0] * H[2];
		float S = C[2] * P[0] - C[0] * P[2];

		glm::vec2 t = intersection.positionRelative = { -R / S, -X / S };
		//intersection.exists = t[0] >= 0 && t[0] <= 1 && t[1] >= 0 && t[1] <= 1;
		intersection.exists = t[0] > 0.f && t[0] < 1.f && t[1] > 0.f && t[1] < 1.f;
		return intersection;
		//--------------------------------------------------------------------
	}

	return intersection;
}

std::vector<StreetSegmentIntersection> StreetMap::IntersectionsWithAny(const StreetSegment segment) const
{
	std::vector<StreetSegmentIntersection> intersections = {};
	//	Kontrola lokálních kolizí (tento StreetMap)
	//for (size_t i = ReadStreets().size() - 1; i >= 0 && i != std::numeric_limits<size_t>::max(); i--)
	for (size_t i = 0; i < ReadStreets().size(); i++)
	{
		//	Načtení ulice pro ověření
		auto street = ReadStreets()[i];
		//	TODO: Vybírat pouze relevantní ulice (nacházející se v blízkosti segmentu)

		//	Kontrola průsečíku
		auto intersection = Intersection(segment, street);
		if (intersection.exists)
		{
			//	Průsečík existuje
			intersections.push_back(std::move(intersection));
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
		if (intersections.size())
		{
			for (size_t j = 0; j < intersections.size(); j++)
			{
				auto intersection = intersections[j];
				auto endPoint = street->GetSegmentPoint(intersection.positionRelative[0] - 0.001f);
				street->SetSegmentEndPoint(endPoint);
			}

			street->End();
			continue;
		}

		float split = glm::floor(segment.length / length);
		if (split > segment.lastSplit && street->GetLevel() < 2)
		{
			segment.lastSplit = split;

			//	TODO: Vytváření nových ulic
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

			auto newStreet = std::make_shared<Infrastructure::Street>(position, direction, length, street->GetLevel() + 1);
			GetStreets().push_back(newStreet);
		}
	}
}
