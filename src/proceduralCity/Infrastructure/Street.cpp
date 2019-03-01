///
/// @file Street.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetNode.h>
#include <Infrastructure/Structs/StreetIntersection.h>
#include <Infrastructure/Structs/StreetSegmentIntersection.h>
#include <Infrastructure/Structs/StreetSegment.h>
#include <Infrastructure/Structs/StreetVertex.h>
#include <Terrain/HeightMap.h>
#include <glm/detail/_noise.hpp>


using namespace ge::gl;
using namespace Infrastructure;

Street::Street(Terrain::HeightMap* heightMap, glm::vec3 const& startPoint, glm::vec3 const& direction,
               const float length, const short level)
	: parentSegment({ glm::vec3(0), glm::vec3(0), glm::vec3(0), 0.f, nullptr }), _heightMap(heightMap), _level(level)
{
	const auto ptr = std::shared_ptr<Street>(this, [](Street*) {});

	SetDrawMode(GL_LINES);

	auto va = CreateVA();
	BindVA();

	auto vb = CreateVB();
	BindVB();

	const StreetSegment newSegment = {
		startPoint,
		startPoint + length * direction,
		direction,
		length,
		ptr,
	};
	StreetRootNode->Insert(newSegment);

	_segments.reserve(16 * sizeof(StreetVertex));
	_vertices.reserve(16 * 2 * sizeof(StreetVertex));

	_segments.push_back(newSegment);
	_vertices.push_back({
		newSegment.startPoint,
	});
	_vertices.push_back({
		newSegment.endPoint,
	});

	vb->alloc(2 * sizeof(StreetVertex));
	BuildStep();
	va->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(StreetVertex));
}

Street::~Street()
{
	Destroy();
}

void Street::Destroy()
{
	const auto thisStreet = this->shared_from_this();

	// TODO: Napojit segmenty
	for (const auto& segment : GetSegments())
		StreetRootNode->Remove(segment);

	if (parentSegment.street)
	{
		parentSegment.street->RemoveSubstreet(thisStreet);
		parentSegment.street->RemoveIntersection(thisStreet);
		parentSegment.street = nullptr;
	}

	for (const auto& intersection : GetIntersections())
		intersection.intersecting_segment.street->RemoveIntersection(thisStreet);
}


StreetSegment const& Street::ReadSegment() const
{
	assert(!_segments.empty());
	return _segments[_segments.size() - 1];
}

StreetSegment const& Street::ReadSegment(const size_t segment) const
{
	assert(segment >= 0);
	assert(segment < _segments.size());
	return _segments[segment];
}

StreetSegment const& Street::GetSegment()
{
	assert(!_segments.empty());
	return _segments[_segments.size() - 1];
}

StreetSegment const& Street::GetSegment(const size_t segment)
{
	assert(segment >= 0);
	assert(segment < _segments.size());
	return _segments[segment];
}


glm::vec3 Street::GetSegmentPoint(const float t) const
{
	const auto seg = ReadSegment(_segments.size() - 1);
	return (1.f - t) * seg.startPoint + t * seg.endPoint;
}

glm::vec3 Street::GetSegmentPoint(const size_t segment, const float t) const
{
	const auto seg = ReadSegment(segment);
	return (1.f - t) * seg.startPoint + t * seg.endPoint;
}

void Street::SetSegmentEndPoint(glm::vec3 const& endPoint)
{
	assert(GetVB() != nullptr);
	_segments.back().endPoint = endPoint;
	_vertices.back().position = endPoint;
	GetVB()->setData(&_vertices[0]);
}

void Street::ResetSegmentSplit()
{
	lengthSplit = 0.f;
}

void Street::BuildStep()
{
	BuildStep(ReadSegment().direction, ReadSegment().length);
}

void Street::BuildStep(glm::vec3 const& direction)
{
	BuildStep(direction, ReadSegment().length);
}

void Street::BuildStep(const float length)
{
	BuildStep(ReadSegment().direction, length);
}

void Street::BuildStep(glm::vec3 const& direction, const float length)
{
	assert(GetVB() != nullptr);
	assert(!_segments.empty());
	assert(!_vertices.empty());

	lengthSplit += length;
	if (false && direction == ReadSegment().direction)
	{
		//	TODO: Odstranit, aktualizovat referencí?
		StreetRootNode->Remove(ReadSegment());

		//	Směrnice jsou stejné, pouze prodloužíme původní úsečku
		_segments.back().endPoint = _segments.back().endPoint + length * direction;
		_segments.back().length += length;
		_vertices.back().position = _segments.back().endPoint;

		//	TODO: Odstranit, aktualizovat referencí?
		StreetRootNode->Insert(ReadSegment());
	}
	else
	{
		//	Směrnice nejsou stejné, přidáváme nový intersectingSegment a vertexy
		StreetSegment newSegment{
			ReadSegment().endPoint,
			ReadSegment().endPoint + length * direction,
			direction,
			length,
			shared_from_this(),
		};

		newSegment.endPoint.y = _heightMap->GetData(newSegment.endPoint) + 0.2f;
		//std::cerr << glm::abs(newSegment.startPoint.y - newSegment.endPoint.y) << std::endl;
		if (glm::abs(newSegment.startPoint.y - newSegment.endPoint.y) >= .75f)
		{
			this->End();
			return;
		}

		//	Uložení nového segmentu
		const auto segmentsSize = _segments.max_size();
		StreetRootNode->Insert(newSegment);
		_segments.push_back(newSegment);/*
		if (segmentsSize < _segments.max_size())
		{
			std::cerr << "Segment vector resized!" << std::endl;
			StreetSegment segment0{ glm::vec3(0), glm::vec3(0), glm::vec3(0), 0 };
			std::fill(_segments.begin() + _segments.size(), _segments.end(), segment0);
		}*/

		//	Uložení nových vertexů
		const auto verticesSize = _segments.max_size();
		_vertices.push_back({
			newSegment.startPoint,
		});
		_vertices.push_back({
			newSegment.endPoint,
		});
		SetRenderableCount(_vertices.size());
		/*
		if (verticesSize < _vertices.max_size())
		{
			std::cerr << "Vertex vector resized!" << std::endl;
			StreetVertex vertex0{ glm::vec3(0) };
			std::fill(_vertices.begin() + _vertices.size(), _vertices.end(), vertex0);
		}*/

		//	Kontrola velikosti bufferu
		if (_vertices.size() * sizeof(StreetVertex) > GetVB()->getSize())
		{
			//	Zdvojnásobení aktuální velikosti bufferu
			GetVB()->realloc(GetVB()->getSize() * 2, Buffer::ReallocFlags::KEEP_ID);
		}
	}

	//	Aktualizace dat v bufferu
	GetVB()->setData(_vertices.data());
}

void Street::AddSubstreet(StreetSegment const& source_segment, std::shared_ptr<Street> const& substreet)
{
	_substreets.push_back(substreet);
	substreet->parentSegment = source_segment;
}

void Street::RemoveSubstreet(const std::shared_ptr<Street>& substreet)
{
	_substreets.erase(std::remove(_substreets.begin(), _substreets.end(), substreet), _substreets.end());
}

std::vector<StreetIntersection> const& Street::GetIntersections() const
{
	return _intersections;
}

StreetIntersectionSide Street::GetPointSide(glm::vec3 const& point, StreetSegment const& segment) const
{
	const auto a = segment.startPoint;
	const auto b = segment.endPoint;
	const auto p = point;

	//--------------------------------------------------------------------
	// https://math.stackexchange.com/a/274728
	//
	const auto d = (p.x - a.x) * (b.z - a.z) - (p.z - a.z) * (b.x - a.x);
	//--------------------------------------------------------------------

	if (d < 0)
		return RIGHT;
	
	return LEFT;
}

void Street::AddIntersection(glm::vec3 const& intersection_point, StreetSegment const& intersecting_segment, StreetSegment const& own_segment)
{
	const auto intersection_side = GetPointSide(intersecting_segment.startPoint, own_segment);
	AddIntersection(intersection_point, intersecting_segment, intersection_side, own_segment);
}

void Street::AddIntersection(glm::vec3 const& intersection_point, StreetSegment const& intersecting_segment, const StreetIntersectionSide intersection_side, StreetSegment const& own_segment)
{
	const auto intersecting_street = intersecting_segment.street;
	const auto is_substreet = std::find(_substreets.begin(), _substreets.end(), intersecting_street) != _substreets.end();
	_intersections.push_back({
		intersection_point,
		intersection_side,
		is_substreet,
		intersecting_segment,
		own_segment,
	});

	const auto start_point = GetSegment(0).startPoint;
	std::sort(_intersections.begin(), _intersections.end(),
		[&](StreetIntersection const& intersection1, StreetIntersection const& intersection2)
	{
		return glm::distance(start_point, intersection1.point) < glm::distance(start_point, intersection2.point);
	});
}

void Street::RemoveIntersection(std::shared_ptr<Street> const& street)
{
	if (!_intersections.empty())
	{
		_intersections.erase(std::remove_if(_intersections.begin(), _intersections.end(),
			[&](StreetIntersection const& intersection)
		{
			return intersection.intersecting_segment.street == street;
		}), _intersections.end());
	}
}

void Street::GenerateIntersectionPointLists()
{
	_intersectionPointsLeft.clear();
	_intersectionPointsRight.clear();

	auto sourcePointLeft = GetSegment(0).startPoint;
	auto sourcePointRight = sourcePointLeft;

	auto lastSegmentLeft = parentSegment;
	auto lastSegmentRight = parentSegment;

	for (const auto& intersection : GetIntersections())
	{
		if (intersection.side == LEFT)
		{
			_intersectionPointsLeft.push_back({
				sourcePointLeft,
				intersection.point,
				lastSegmentLeft,
				intersection.intersecting_segment,
				!intersection.is_substreet,
			});
			sourcePointLeft = intersection.point;
			lastSegmentLeft = intersection.intersecting_segment;
		}
		else if (intersection.side == RIGHT)
		{
			_intersectionPointsRight.push_back({
				sourcePointRight,
				intersection.point,
				lastSegmentRight,
				intersection.intersecting_segment,
				!intersection.is_substreet,
			});
			sourcePointRight = intersection.point;
			lastSegmentRight = intersection.intersecting_segment;
		}
	}
}

std::vector<StreetNarrowPair> const& Street::GetLeftIntersectionPointPairs() const
{
	return _intersectionPointsLeft;
}

std::vector<StreetNarrowPair> const& Street::GetRightIntersectionPointPairs() const
{
	return _intersectionPointsRight;
}

StreetNarrowPair const& Street::GetNextIntersectionPointPair(StreetNarrowPair const& currentPair, bool wasInverted)
{
	StreetIntersectionSide side;
	/*
	if (wasInverted)
		side = GetPointSide(currentPair.point2, currentPair.intersecting_segment1);
	else*/
		side = GetPointSide(currentPair.point1, currentPair.intersecting_segment2);
	if (side == LEFT)
	{
		auto nextPair = std::find_if(_intersectionPointsLeft.begin(), _intersectionPointsLeft.end(),
			[&](StreetNarrowPair const& pair)
		{/*
			if (wasInverted)
				return currentPair.point1 == pair.point2;*/

			return currentPair.point2 == pair.point2;
		});
		if (nextPair != _intersectionPointsLeft.end())
			return *nextPair;

		/*
		nextPair = std::find_if(_intersectionPointsLeft.begin(), _intersectionPointsLeft.end(),
			[&](StreetNarrowPair const& pair)
		{
			return currentPair.point2 == pair.point1;
		});
		if (nextPair != _intersectionPointsLeft.end())
		{
			std::swap(nextPair->point1, nextPair->point2);
			std::swap(nextPair->intersecting_segment1, nextPair->intersecting_segment2);
			return *nextPair;
		}
		 
		*/
	}
	else if (side == RIGHT)
	{
		auto nextPair =  std::find_if(_intersectionPointsRight.begin(), _intersectionPointsRight.end(),
			[&](StreetNarrowPair const& pair)
		{/*
			if (wasInverted)
				return currentPair.point1 == pair.point1;*/
			return currentPair.point2 == pair.point1;
		});
		if (nextPair != _intersectionPointsRight.end())
			return *nextPair;

		/*
		nextPair = std::find_if(_intersectionPointsRight.begin(), _intersectionPointsRight.end(),
			[&](StreetNarrowPair const& pair)
		{
			return currentPair.point2 == pair.point2;
		});
		if (nextPair != _intersectionPointsRight.end())
		{
			std::swap(nextPair->point1, nextPair->point2);
			std::swap(nextPair->intersecting_segment1, nextPair->intersecting_segment2);
			return *nextPair;
		}
		 
		 */
	}

	return currentPair;
}
