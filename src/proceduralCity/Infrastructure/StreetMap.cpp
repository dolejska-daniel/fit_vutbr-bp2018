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

float stepSize = 4.f;
float stepLevelOffset = .5f;


StreetMap::StreetMap()
{
	if (StreetRootNode == nullptr)
		throw std::runtime_error("StreetRootNode is nullptr.");

	auto street = std::make_shared<Street>(glm::vec3(0, 4, 0), glm::vec3(1, 0, 0), stepSize / 2);
	StreetRootNode->Insert(street->GetSegment());
	GetStreets().push_back(street);

	street = std::make_shared<Street>(glm::vec3(0, 4, 0), glm::vec3(-.5, 0, -.5), stepSize / 2);
	StreetRootNode->Insert(street->GetSegment());
	GetStreets().push_back(street);

	street = std::make_shared<Street>(glm::vec3(0, 4, 0), glm::vec3(-.5, 0, .5), stepSize / 2);
	StreetRootNode->Insert(street->GetSegment());
	GetStreets().push_back(street);

	/*
	auto street = std::make_shared<Street>(glm::vec3(-5, 4, 0), glm::vec3(2.5f, 0, 7.5f), stepSize / 2);
	StreetRootNode->Insert(street->GetSegment());
	GetStreets().push_back(street);

	street = std::make_shared<Street>(glm::vec3(10, 4, 0), glm::vec3(-.25f, 0, .75f), stepSize / 2);
	StreetRootNode->Insert(street->GetSegment());
	GetStreets().push_back(street);
	*/
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
	if (segment1.direction == segment2.direction
		/*|| segment1.direction == -segment2.direction*/)
	{
		//	Segmenty jsou rovnoběžné, kolize není možná
		return  {
			false,
			glm::vec2(0),
		};
	}

	//--------------------------------------------------------------------
	//	Tomáš Milet
	//
	auto c = segment1.endPoint - segment1.startPoint;
	auto p = segment2.startPoint - segment2.endPoint;
	auto h = segment1.startPoint - segment2.startPoint;

	const auto r = h[2] * p[0] - h[0] * p[2];
	const auto x = c[2] * h[0] - c[0] * h[2];
	const auto s = c[2] * p[0] - c[0] * p[2];

	glm::vec2 t{ -r / s, -x / s };
	//intersection.exists = t[0] >= 0 && t[0] <= 1 && t[1] >= 0 && t[1] <= 1;
	return {
		t[0] > 0.f && t[0] < 1.f && t[1] > 0.f && t[1] < 1.f,
		t,
	};
	//--------------------------------------------------------------------

	//printf("ex? %d, vec2(%10f, %10f)\n", intersection.exists, intersection.positionRelative);
}

std::shared_ptr<std::vector<StreetSegmentIntersection>> StreetMap::Intersections(StreetSegment const& segment) const
{
	auto intersections = std::make_shared<std::vector<StreetSegmentIntersection>>();
	Intersections(segment, StreetRootNode, intersections);
	return intersections;
}

void StreetMap::Intersections(StreetSegment const& segment, std::shared_ptr<StreetNode> const& node, std::shared_ptr<std::vector<StreetSegmentIntersection>> const& intersections) const
{
	for (auto const& streetSegment : node->GetSegments())
	{
		auto intersection = Intersection(segment, streetSegment);
		if (intersection.exists)
		{
			//	Průsečík existuje
			intersections->push_back(intersection);
		}
	}

	const auto position = node->RelativePositionFor(segment);
	if (position == StreetNode::RelativePosition::NONE)
	{
		//	Nepatří jednoznačně do žádného z poduzlů
		for (auto const& childNode : node->GetChildren())
			Intersections(segment, childNode.second, intersections);

		return;
	}

	const auto childNode = node->GetChildren().find(position);
	if (childNode != node->GetChildren().end())
		return Intersections(segment, childNode->second, intersections);
}

void StreetMap::BuildStep()
{
	for (auto const& street : ReadStreets())
	{
		if (street->Ended())
			continue;

		float length = glm::pow(stepLevelOffset, street->GetLevel()) * stepSize;
		street->BuildStep(length);

		auto segment = street->GetSegment();
		auto intersections = Intersections(segment);
		if (!intersections->empty())
		{
			//	Průsečíky byly nalezeny
			auto intersectionPoint = street->GetSegmentPoint(intersections->begin()[0].positionRelative[0]);
			auto intersectionDistance = distance(intersectionPoint, segment.startPoint);

			for (auto intersection : *intersections)
			{
				const auto intersectionPoint_ = street->GetSegmentPoint(intersection.positionRelative[0]);
				const auto intersectionDistance_ = distance(intersectionPoint_, segment.startPoint);

				if (intersectionDistance_ < intersectionDistance)
				{
					intersectionDistance = intersectionDistance_;
					intersectionPoint    = intersectionPoint_;
				}
			}

			//	TODO: tento segment není uložen v quadtree
			StreetRootNode->Remove(street->GetSegment());
			street->SetSegmentEndPoint(intersectionPoint);
			StreetRootNode->Insert(street->GetSegment());
			street->End();
			continue;
		}

		const int split = glm::floor(segment.length / length);
		if (split > segment.lastSplit && street->GetLevel() < 3 && std::rand() % 100 <= 3) // 4 dobrá, 3 velmi dobrá
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
