///
/// @file StreetMap.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/mat2x2.hpp>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetMap.h>
#include <Infrastructure/StreetNode.h>


using namespace Infrastructure;

float stepSize = 10.f;
float stepLevelOffset = .5f;


StreetMap::StreetMap()
{
	auto street = std::make_shared<Street>(glm::vec3(0, 4, 0), glm::vec3(1, 0, 0), stepSize / 2);
	StreetRootNode->Insert(street->GetSegment());
	GetStreets().push_back(street);

	street = std::make_shared<Street>(glm::vec3(0, 4, 0), glm::vec3(-.5, 0, -.5), stepSize / 2);
	StreetRootNode->Insert(street->GetSegment());
	GetStreets().push_back(street);

	street = std::make_shared<Street>(glm::vec3(0, 4, 0), glm::vec3(-.5, 0, .5), stepSize / 2);
	StreetRootNode->Insert(street->GetSegment());
	GetStreets().push_back(street);
}

StreetMap::~StreetMap()
= default;


StreetSegmentIntersection StreetMap::Intersection(StreetSegment const& segment, std::shared_ptr<Street> const& street)
{
	auto streetSegments = street->GetSegments();
	for (auto streetSegment : streetSegments)
	{
		auto intersection = Intersection(segment, streetSegment);
		if (intersection.exists)
			return intersection;
	}

	return {
		false,
		glm::vec3(0),
	};
}

StreetSegmentIntersection StreetMap::Intersection(StreetSegment const& segment1, StreetSegment const& segment2)
{
	if (segment1.direction == segment2.direction)
	{
		//	Segmenty jsou rovnoběžné, kolize není možná
		return  {
			false,
			glm::vec3(0),
		};
	}

	StreetSegmentIntersection intersection = {
		false,
		glm::vec3(0),
	};

	//--------------------------------------------------------------------
	//	Tomáš Milet
	//
	auto c = segment1.endPoint - segment1.startPoint;
	auto p = segment2.startPoint - segment2.endPoint;
	auto h = segment1.startPoint - segment2.startPoint;

	const auto r = h[2] * p[0] - h[0] * p[2];
	const auto x = c[2] * h[0] - c[0] * h[2];
	const auto s = c[2] * p[0] - c[0] * p[2];

	auto t = intersection.positionRelative = { -r / s, -x / s };
	//intersection.exists = t[0] >= 0 && t[0] <= 1 && t[1] >= 0 && t[1] <= 1;
	intersection.exists = t[0] > 0.f && t[0] < 1.f && t[1] > 0.f && t[1] < 1.f;
	//--------------------------------------------------------------------

	return intersection;
}

std::vector<StreetSegmentIntersection> StreetMap::IntersectionsWithAny(StreetSegment const& segment) const
{
	std::vector<StreetSegmentIntersection> intersections = {};

	const auto containingNode = StreetRootNode->GetContainingNode(segment);
	for (auto const& streetSegment : containingNode->ReadSegments())
	{
		auto intersection = Intersection(segment, streetSegment);
		if (intersection.exists)
		{
			//	Průsečík existuje
			intersections.push_back(intersection);
		}
	}

	return intersections;
}

void StreetMap::BuildStep()
{
	for (auto const& street : ReadStreets())
	{
		if (street->Ended())
			continue;

		StreetRootNode->Remove(street->GetSegment());
		float length = glm::pow(stepLevelOffset, street->GetLevel()) * stepSize;
		street->BuildStep(length);
		StreetRootNode->Insert(street->GetSegment());

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
