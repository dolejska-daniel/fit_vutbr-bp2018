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


//const float error = 0.0025f;
const float error = 0.0f;

StreetMap::StreetMap(Terrain::Map *map)
	: terrainMap(map)
{
	if (StreetRootNode == nullptr)
		throw std::runtime_error("StreetRootNode is nullptr.");

	_zone = std::make_shared<StreetZone>(map->GetVars(), glm::vec2(0.f, 0.f), INFINITY);
	/*
	for (auto i = 0; i < 10; i++)
		_zone->Add(std::make_shared<StreetZone>(map->GetVars(), glm::vec2(400 - (rand() % 800), 400 - (rand() % 800)), 50.f, [&](std::shared_ptr<Street> const& street)
		{
			auto dir = street->ReadSegment().direction;
			//if (street->GetLevel() < 2)
			//	dir += glm::vec3(0, 0, 0.05f);
			//dir = glm::normalize(dir);
			street->BuildStep(dir, float(glm::pow(.75f, street->GetLevel()) * 6.f));
		})); */

	/*
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
	*/

	auto startPoint1 = glm::vec3(-200, 0, 50);
	startPoint1.y = map->GetHeightMap()->GetData(startPoint1) + .2f;
	
	auto street = std::make_shared<Street>(map->GetHeightMap(), startPoint1, glm::vec3(1.f, 0, 0), 2.f);
	StreetRootNode->Insert(street->ReadSegment());
	GetStreets().push_back(street);


	auto startPoint2 = glm::vec3(-175, 0, 30);
	startPoint2.y = map->GetHeightMap()->GetData(startPoint2) + .2f;

	street = std::make_shared<Street>(map->GetHeightMap(), startPoint2, glm::vec3(0, 0, 1.f), 2.f);
	StreetRootNode->Insert(street->ReadSegment());
	GetStreets().push_back(street);
}

StreetMap::~StreetMap()
= default;


void StreetMap::AddStreet(const std::shared_ptr<Street>& street)
{
	_streets.push_back(street);
}

void StreetMap::RemoveStreet(const std::shared_ptr<Street>& street)
{
	street->Destroy(shared_from_this());
	_streets.erase(std::remove(_streets.begin(), _streets.end(), street), _streets.end());
}

StreetSegmentIntersection StreetMap::Intersection(StreetSegment const& segment, std::shared_ptr<Street> const& street)
{
	auto street_segments = street->GetSegments();
	for (const auto& street_segment : street_segments)
	{
		auto intersection = Intersection(segment, street_segment);
		if (intersection.exists)
		{
			intersection.intersectingSegment = segment;
			intersection.ownSegment = street_segment;
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
	// kolize v rámci stejné silnice není možná
	if (segment1.street == segment2.street)
		return { false };

	/*
	std::cerr << "\n===\nEvaluating intersection of\n\t" << glm::to_string(segment1.startPoint) << " to " << glm::to_string(segment1.endPoint) << " in " << glm::to_string(segment1.direction) << "\n\t" << glm::to_string(segment2.startPoint) << " to " << glm::to_string(segment2.endPoint) << " in " << glm::to_string(segment2.direction) << std::endl;
	std::cerr << "(" << segment1.startPoint.x << ", " << segment1.startPoint.z << ")" << std::endl;
	std::cerr << "(" << segment1.endPoint.x << ", " << segment1.endPoint.z << ")" << std::endl;
	std::cerr << "(" << segment2.startPoint.x << ", " << segment2.startPoint.z << ")" << std::endl;
	std::cerr << "(" << segment2.endPoint.x << ", " << segment2.endPoint.z << ")" << std::endl;
	*/
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
	//std::cerr << "Intersection found at " << glm::to_string(t) << std::endl;
	//std::cerr << "\tr: " << r << "\n\tx: " << x << "\n\ts: " << s << std::endl;
	return {
		   t[0] >= 0.f + error && t[0] <= 1.f - error
		&& t[1] >= 0.f + error && t[1] <= 1.f - error,
		t,
	};
	//--------------------------------------------------------------------
}

std::shared_ptr<std::vector<StreetSegmentIntersection>> StreetMap::Intersections(StreetSegment const& segment) const
{
	auto intersections = std::make_shared<std::vector<StreetSegmentIntersection>>();
	Intersections(segment, StreetRootNode, intersections);
	return intersections;
}

void StreetMap::Intersections(StreetSegment const& segment, std::shared_ptr<StreetNode> const& node, std::shared_ptr<std::vector<StreetSegmentIntersection>> const& intersections) const
{
	for (auto const& street_segment : node->GetSegments())
	{
		auto intersection = Intersection(segment, street_segment);
		if (intersection.exists)
		{
			//	Průsečík existuje
			intersection.intersectingSegment = segment;
			intersection.ownSegment = street_segment;
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

void StreetMap::ValidateIntersections()
{
	auto streets = GetStreets();
	for (auto i = streets.rbegin(); i != streets.rend(); ++i)
		ValidateIntersections(*i);
}

void StreetMap::ValidateIntersections(const std::shared_ptr<Street>& street)
{
	const auto err = 0.001f;
	std::cerr << "\n===\nValidating street: " << street << std::endl;
	auto segments = street->GetSegments();
	for (const auto& other_street : GetStreets())
	{
		if (other_street == street)
			continue;
		if (other_street->HasSubstreet(street))
		{
			std::cerr << "Street " << street << " is substreet of " << other_street << ", skipping..." << std::endl;
			continue;
		}
		if (street->HasSubstreet(other_street))
		{
			std::cerr << "Street " << other_street << " is substreet of " << street << ", skipping..." << std::endl;
			continue;
		}

		for (size_t i = 0; i < segments.size(); ++i)
		{
			auto street_segment = segments[i];
			const auto intersection = Intersection(street_segment, other_street);
			if (intersection.exists)
			{
				auto point_position = intersection.positionRelative[0];
				std::cerr << "Street intersects: " << street << " with " << other_street << " at " << point_position << std::endl;
				if (std::abs(point_position) < err || std::abs(point_position) > 1.f - err)
				{
					//std::cerr << "  Intersection at: " << point_position << std::endl;
					continue;
				}

				auto point = street_segment.GetPoint(point_position);
				std::cerr << "  Validating existing intersections against: " << glm::to_string(point) << std::endl;
				if (std::find_if(street->GetIntersections().begin(), street->GetIntersections().begin(), [&](StreetIntersection existing_intersection)
				{
					return existing_intersection.point == point;
				}) != street->GetIntersections().end())
				{
					std::cerr << "  Intersection already exists, skipping..." << std::endl;
					continue;
				}

				std::cerr << "Intersection was not found!" << std::endl;
				std::cerr << "Cropping street segment " << i << " to " << glm::to_string(point) << std::endl;
				street->SetSegmentEndPoint(i, point);
				std::cerr << "Removing segments from " << i + 1 << std::endl;
				street->RemoveSegmentsToEnd(i + 1);
				street->End(point, intersection.ownSegment, intersection.intersectingSegment);
				other_street->AddIntersection(point, intersection.intersectingSegment, intersection.ownSegment);

				// TODO: validate substreets, intersections
				return;
			}
		}
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
			street->End(intersectionPoint, intersection.ownSegment, intersection.intersectingSegment);
			intersection.ownSegment.street->AddIntersection(intersectionPoint, intersection.intersectingSegment, intersection.ownSegment);

			intersecting_segment = street->GetSegment();
			continue;
		}

		//	TODO: SplitStep
		_zone->SplitStep(this, street);

	}
}
