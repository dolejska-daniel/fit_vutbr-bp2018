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

StreetMap::StreetMap()
{
	auto street = std::make_shared<Infrastructure::Street>(glm::vec3(10, 4, 0), glm::vec3(-1, 0, 0), 1);
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

	auto LiesWithin = [](glm::vec3 point, glm::vec2 min, glm::vec2 max) -> bool {
		return point.x >= min.x && point.z >= min.y
			&& point.x <= max.x && point.z <= max.y;
	};

	glm::vec2 segment1Min = {
		glm::min(segment.startPoint.x, segment.endPoint.x),
		glm::min(segment.startPoint.z, segment.endPoint.z),
	};
	glm::vec2 segment1Max = {
		glm::max(segment.startPoint.x, segment.endPoint.x),
		glm::max(segment.startPoint.z, segment.endPoint.z),
	};

	//--------------------------------------------------------------------
	//	http://mathworld.wolfram.com/Line-LineIntersection.html
	//
	glm::mat2x2 segment1 = {
		segment.startPoint.x,	segment.startPoint.z,
		segment.endPoint.x,		segment.endPoint.z,
	};
	float segment1Determinant = glm::determinant(segment1);
	//--------------------------------------------------------------------

	auto streetSegments = street->GetSegments();
	for (size_t i = 0; i < streetSegments.size(); i++)
	{
		auto streetSegment = streetSegments[i];
		printf("\twith vec3(%12.10f, %12.10f, %12.10f), vec3(%12.10f, %12.10f, %12.10f)\n",
			streetSegment.startPoint.x, streetSegment.startPoint.y, streetSegment.startPoint.z,
			streetSegment.endPoint.x, streetSegment.endPoint.y, streetSegment.endPoint.z);

		if (segment.direction == streetSegment.direction)
		{
			//	Segmenty jsou rovnoběžné, kolize není možná
			printf("\t\tSame slope - intersection impossible!\n");
			continue;
		}

		if (false && LiesWithin(streetSegment.startPoint, segment1Min, segment1Max) == false && LiesWithin(streetSegment.endPoint, segment1Min, segment1Max) == false)
		{
			//	Počáteční ani koncové body druhého segmentu neleží v mezích prvního segmentu
			glm::vec2 segment2Min = {
				glm::min(streetSegment.startPoint.x, streetSegment.endPoint.x),
				glm::min(streetSegment.startPoint.z, streetSegment.endPoint.z),
			};
			glm::vec2 segment2Max = {
				glm::max(streetSegment.startPoint.x, streetSegment.endPoint.x),
				glm::max(streetSegment.startPoint.z, streetSegment.endPoint.z),
			};
			if (LiesWithin(segment.startPoint, segment2Min, segment2Max) == false && LiesWithin(segment.endPoint, segment2Min, segment2Max) == false)
			{
				//	Počáteční ani koncové body prvního segmentu neleží v mezích druhého segmentu, segmenty se neprotínají
				printf("\t\tPoints mutually independent!\n");
				continue;
			}
		}

		//--------------------------------------------------------------------
		//	http://mathworld.wolfram.com/Line-LineIntersection.html
		//
		//	TODO: Přesunout výpočet determinantu do vytváření segmentu
		glm::mat2x2 segment2 = {
			streetSegment.startPoint.x, streetSegment.startPoint.z,
			streetSegment.endPoint.x,	streetSegment.endPoint.z,
		};
		float segment2Determinant = glm::determinant(segment2);

		glm::mat2x2 bottom = {
			segment.startPoint.x - segment.endPoint.x,				segment.startPoint.z - segment.endPoint.z,
			streetSegment.startPoint.x - streetSegment.endPoint.x,	streetSegment.startPoint.z - streetSegment.endPoint.z,
		};
		float bottomDeterminant = glm::determinant(bottom);

		glm::mat2x2 x = {
			segment1Determinant, segment.startPoint.x - segment.endPoint.x,
			segment2Determinant, streetSegment.startPoint.x - streetSegment.endPoint.x,
		};
		float xDeterminant = glm::determinant(x);

		glm::mat2x2 y = {
			segment1Determinant, segment.startPoint.z - segment.endPoint.z,
			segment2Determinant, streetSegment.startPoint.z - streetSegment.endPoint.z,
		};
		float yDeterminant = glm::determinant(y);

		glm::vec3 intersectionPoint = {
			xDeterminant / bottomDeterminant,
			segment.startPoint.y,
			yDeterminant / bottomDeterminant,
		};
		//--------------------------------------------------------------------

		if (LiesWithin(intersectionPoint, segment1Min, segment1Max))
		{
			//	FIXME: Nepřesnosti ve výpočtech??

			printf("\t\tIntersection detected!\n");
			printf("\t\t\tvec3(%12.10f, %12.10f, %12.10f)\n", intersectionPoint.x, intersectionPoint.y, intersectionPoint.z);
			if (glm::round(intersectionPoint) == glm::round(segment.startPoint)
				|| glm::round(intersectionPoint) == glm::round(streetSegment.startPoint))
			{
				printf("\t\t\t --- Start point ---\n");
				continue;
			}

			//	Výsledný průsečík leží v mezích segmentu
			intersection.exists = true;
			intersection.position = intersectionPoint;
		}
	}

	return intersection;
}

std::vector<StreetSegmentIntersection> StreetMap::IntersectionsWithAny(const StreetSegment segment) const
{
	printf("Looking for intersections:\n");
	printf("\tfor  vec3(%12.10f, %12.10f, %12.10f), vec3(%12.10f, %12.10f, %12.10f)\n",
		segment.startPoint.x, segment.startPoint.y, segment.startPoint.z,
		segment.endPoint.x, segment.endPoint.y, segment.endPoint.z);

	std::vector<StreetSegmentIntersection> intersections = {};
	//	Kontrola lokálních kolizí (tento StreetMap)
	//for (size_t i = ReadStreets().size() - 1; i >= 0 && i != std::numeric_limits<size_t>::max(); i--)
	for (size_t i = 0; i < ReadStreets().size(); i++)
	{
		//	Načtení ulice pro ověření
		auto street = ReadStreets()[i];
		if (street->Ended())
			continue;
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
	// for (auto iterator = std::rbegin(GetStreets()); iterator != std::rend(GetStreets()); ++iterator)
	float stepSize = .2f;

	//for (size_t i = GetStreets().size() - 1; i >= 0 && i != std::numeric_limits<size_t>::max(); i--)
	for (size_t i = 0; i < GetStreets().size(); i++)
	{
		auto street = GetStreets()[i];
		if (street->Ended())
			continue;

		street->BuildStep(stepSize);

		auto streetSegment = street->GetSegment();
		auto intersections = IntersectionsWithAny(streetSegment);
		if (intersections.size())
		{
			printf("\tSome intersections found!\n");
			for (size_t j = 0; j < intersections.size(); j++)
			{
				auto intersection = intersections[j];
				printf("\t\tvec3(%12.10f, %12.10f, %12.10f)\n\n", intersection.position.x, intersection.position.y, intersection.position.z);
			}
			//	TODO: Upravit délku streetSegment

			street->End();
			continue;
		}

		if (std::rand() % 500 <= 1)
		{
			printf("\tCreating new street.\n");
			//	TODO: Vytváření nových ulic
			glm::vec3 position = streetSegment.startPoint + std::roundf(0.5f * streetSegment.length) * streetSegment.direction;
			glm::vec3 direction;

			if (std::rand() % 10 <= 5)
			{
				direction.x = -streetSegment.direction.z;
				direction.y = streetSegment.direction.y;
				direction.z = streetSegment.direction.x;
			}
			else
			{
				direction.x = streetSegment.direction.z;
				direction.y = streetSegment.direction.y;
				direction.z = -streetSegment.direction.x;
			}

			auto newStreet = std::make_shared<Infrastructure::Street>(position, direction, stepSize);
			GetStreets().push_back(newStreet);
		}
	}
}
