///
/// @file StreetMap.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/mat2x2.hpp>
#include <Vars/Vars.h>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetMap.h>
#include <Infrastructure/StreetNode.h>
#include <Infrastructure/StreetZone.h>
#include <Terrain/Map.h>
#include <Terrain/HeightMap.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <algorithm>


using namespace Infrastructure;


const float error = 0.0025f;

StreetMap::StreetMap(Terrain::Map *map)
	: terrainMap(map)
{
	if (StreetRootNode == nullptr)
		throw std::runtime_error("StreetRootNode is nullptr.");

	_zone = std::make_shared<StreetZone>(map->GetVars(), glm::vec2(0.f, 0.f), INFINITY);
	for (auto i = 0; i < 10; i++)
		_zone->Add(std::make_shared<StreetZone>(map->GetVars(), glm::vec2(400 - (rand() % 800), 400 - (rand() % 800)), 50.f, [&](std::shared_ptr<Street> const& street)
		{
			auto dir = street->ReadSegment().direction;
			if (street->GetLevel() < 2)
				dir += glm::vec3(0, 0, 0.05f);
			dir = glm::normalize(dir);
			street->BuildStep(dir, float(glm::pow(.75f, street->GetLevel()) * 6.f));
		}));

	auto startPoint = glm::vec3(0, 0, 32 * 8);
	startPoint.y = map->GetHeightMap()->GetData(startPoint);
	
	auto street = std::make_shared<Street>(map->GetHeightMap(), startPoint, glm::vec3(1, 0, 0), 2.f);
	StreetRootNode->Insert(street->ReadSegment());
	GetStreets().push_back(street);

	street = std::make_shared<Street>(map->GetHeightMap(), startPoint, glm::vec3(-.5, 0, -.5), 2.f);
	StreetRootNode->Insert(street->ReadSegment());
	GetStreets().push_back(street);

	street = std::make_shared<Street>(map->GetHeightMap(), startPoint, glm::vec3(-.5, 0, .5), 2.f);
	StreetRootNode->Insert(street->ReadSegment());
	GetStreets().push_back(street);

	/*
	auto street = std::make_shared<Street>(map->GetHeightMap(), glm::vec3(-5, 4, 0), glm::vec3(2.5f, 0, 7.5f), 2.f);
	StreetRootNode->Insert(street->ReadSegment());
	GetStreets().push_back(street);

	street = std::make_shared<Street>(map->GetHeightMap(), glm::vec3(10, 4, 0), glm::vec3(-.25f, 0, .75f), 2.f);
	StreetRootNode->Insert(street->ReadSegment());
	GetStreets().push_back(street);
	*/
}

StreetMap::~StreetMap()
= default;


void StreetMap::AddStreet(const std::shared_ptr<Street>& street)
{
	_streets.push_back(street);
}

void StreetMap::RemoveStreet(const std::shared_ptr<Street>& street)
{
	_streets.erase(std::remove(_streets.begin(), _streets.end(), street), _streets.end());
}

StreetSegmentIntersection StreetMap::Intersection(StreetSegment const& segment, std::shared_ptr<Street> const& street)
{
	auto street_segments = street->GetSegments();
	for (const auto& street_segment : street_segments)
	{
		auto intersection = Intersection(segment, street_segment);
		//std::cerr << glm::to_string(intersection.positionRelative) << std::endl;
		if (intersection.exists)
		{
			intersection.segment = street_segment;
			return intersection;
		}
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
			glm::vec2(-1),
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
		   t[0] >= 0.f + error && t[0] <= 1.f - error
		&& t[1] >= 0.f + error && t[1] <= 1.f - error,
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
			intersection.segment = streetSegment;
			intersections->push_back(intersection);
		}
	}

	const auto position = node->RelativePositionFor(segment);
	if (position == StreetNode::RelativePosition::NONE)
	{
		// Nepatří jednoznačně do žádného z poduzlů
		for (auto const& childNode : node->GetChildren())
			Intersections(segment, childNode.second, intersections);
	}
	else
	{
		// Patří do některého z poduzlů
		const auto childNode = node->GetChildren().find(position);
		if (childNode != node->GetChildren().end())
			return Intersections(segment, childNode->second, intersections);
	}
}

void StreetMap::BuildStep()
{
	for (auto const& street : ReadStreets())
	{
		if (street->Ended())
			continue;

		auto lastEndPoint = street->GetSegment().endPoint;
		//	TODO: BuildStep
		_zone->BuildStep(street);
		terrainMap->ValidateStreet(street);

		auto intersecting_segment = street->GetSegment();
		auto intersections = Intersections(intersecting_segment);
		if (!intersections->empty())
		{
			//	Průsečíky byly nalezeny
			auto intersection = intersections->front();
			auto intersectionPoint = street->GetSegmentPoint(intersections->front().positionRelative[0]);
			auto intersectionDistance = distance(intersectionPoint, lastEndPoint);

			for (auto const& intersection_ : *intersections)
			{
				const auto intersectionPoint_ = street->GetSegmentPoint(intersection_.positionRelative[0]);
				const auto intersectionDistance_ = distance(intersectionPoint_, lastEndPoint);

				if (intersectionDistance_ < intersectionDistance)
				{
					intersectionDistance = intersectionDistance_;
					intersectionPoint    = intersectionPoint_;
					intersection		 = intersection_;
				}
			}

			StreetRootNode->Remove(street->ReadSegment());
			street->SetSegmentEndPoint(intersectionPoint);
			StreetRootNode->Insert(street->ReadSegment());
			street->End();
			intersection.segment.street->AddIntersection(intersectionPoint, intersecting_segment, intersection.segment);

			intersecting_segment = street->GetSegment();
			continue;
		}

		//	TODO: SplitStep
		_zone->SplitStep(this, street);

	}
}
