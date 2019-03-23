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
#include <Infrastructure/Structs/StreetIntersection.h>
#include <Infrastructure/Structs/StreetSegmentIntersection.h>
#include <Infrastructure/Structs/StreetSegment.h>
#include <Infrastructure/Structs/StreetVertex.h>
#include <Terrain/HeightMap.h>
#include <Utils/QuadTree.h>


using namespace ge::gl;
using namespace Infrastructure;

Street::Street(Terrain::HeightMap* heightMap, glm::vec3 const& startPoint, glm::vec3 const& direction,
               const float length, const short level)
	: parentIntersection({}), _heightMap(heightMap), _level(level)
{
	const auto ptr = std::shared_ptr<Street>(this, [](Street*) {});

	SetDrawMode(GL_LINES);

	auto va = CreateVA();
	BindVA();

	auto vb = CreateVB();
	BindVB();

	const StreetSegment new_segment = {
		startPoint,
		startPoint,
		direction,
		length,
		ptr,
	};
	//Utils::StreetQuadTree->Insert(newSegment);

	_segments.reserve(16 * sizeof(StreetVertex));
	_vertices.reserve(16 * 2 * sizeof(StreetVertex));

	_segments.push_back(new_segment);
	_vertices.push_back({
		new_segment.startPoint,
	});
	_vertices.push_back({
		new_segment.startPoint,
	});

	vb->alloc(2 * sizeof(StreetVertex));
	va->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(StreetVertex));
}

Street::~Street()
= default;

void Street::Destroy(const std::shared_ptr<StreetMap>& streetMap)
{
	if (_destroyed) return;
	_destroyed = true;

	const auto this_street = this->shared_from_this();
	std::cerr << "Removing: " << this_street << std::endl;

	// TODO: Napojit segmenty
	for (const auto& segment : GetSegments())
		Utils::StreetQuadTree->Remove(segment);

	// TODO: Správně odstranit substreet a intersection
	if (parentIntersection.intersecting_segment.street
		|| parentIntersection.own_segment.street)
	{
		auto parent_street = parentIntersection.intersecting_segment.street;
		if (parent_street == this_street)
			parent_street = parentIntersection.own_segment.street;

		if (parent_street)
		{
			parent_street->RemoveIntersection(this_street);
			parent_street->RemoveSubstreet(this_street);
		}
	}

	for (const auto& intersection : GetIntersections())
	{
		auto street = intersection.intersecting_segment.street;
		if (street == this_street)
			street = intersection.own_segment.street;

		street->RemoveIntersection(this_street);
		street->Unend();
	}

	for (const auto& sub_street : _substreets)
	{
		if (sub_street)
		{
			std::cerr << "Removing sub: " << sub_street << std::endl;
			streetMap->RemoveStreet(sub_street);
		}
	}
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
	return seg.GetPoint(t);
}

glm::vec3 Street::GetSegmentPoint(const size_t segment, const float t) const
{
	const auto seg = ReadSegment(segment);
	return seg.GetPoint(t);
}

void Street::SetSegmentEndPoint(glm::vec3 const& endPoint)
{
	assert(GetVB() != nullptr);

	_segments.back().endPoint = endPoint;
	_vertices.back().position = endPoint;
	GetVB()->setData(&_vertices[0]);
}

void Street::SetSegmentEndPoint(const size_t segment, glm::vec3 const& endPoint)
{
	assert(segment < _segments.size());

	_segments[segment].endPoint = endPoint;
	RebuildVertices();
}

void Street::RemoveSegmentsToEnd(const size_t segment)
{
	assert(segment < _segments.size());

	_segments.erase(_segments.begin() + segment, _segments.end());
	RebuildVertices();
}

void Street::RebuildVertices()
{
	assert(GetVB() != nullptr);

	_vertices.clear();
	for (const auto& street_segment : _segments)
	{
		_vertices.push_back({
			street_segment.startPoint,
		});
		_vertices.push_back({
			street_segment.endPoint,
		});
	}
	SetRenderableCount(_vertices.size());
	GetVB()->setData(_vertices.data());
}

void Street::ResetSegmentSplit()
{
	lengthSplit = 0.f;
}

void Street::End(glm::vec3 const& intersection_point, StreetSegment const& intersecting_segment,
	StreetSegment const& own_segment)
{
	_ended = true;
	const auto is_substreet = std::find(_substreets.begin(), _substreets.end(), intersecting_segment.street) != _substreets.end();
	AddIntersection(intersection_point, intersecting_segment, LEFT, own_segment, is_substreet);
	AddIntersection(intersection_point, intersecting_segment, RIGHT, own_segment, is_substreet);
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
		Utils::StreetQuadTree->Remove(ReadSegment());

		//	Směrnice jsou stejné, pouze prodloužíme původní úsečku
		_segments.back().endPoint = _segments.back().endPoint + length * direction;
		_segments.back().length += length;
		_vertices.back().position = _segments.back().endPoint;

		//	TODO: Odstranit, aktualizovat referencí?
		Utils::StreetQuadTree->Insert(ReadSegment());
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

		newSegment.endPoint.y = _heightMap->GetData(newSegment.endPoint) + .2f;
		//std::cerr << glm::abs(newSegment.startPoint.y - newSegment.endPoint.y) << std::endl;
		if (glm::abs(newSegment.startPoint.y - newSegment.endPoint.y) >= .75f)
		{
			this->End();
			return;
		}

		//	Uložení nového segmentu
		const auto segments_size = _segments.max_size();
		Utils::StreetQuadTree->Insert(newSegment);
		_segments.push_back(newSegment);

		//	Uložení nových vertexů
		const auto vertices_size = _segments.max_size();
		_vertices.push_back({
			newSegment.startPoint,
		});
		_vertices.push_back({
			newSegment.endPoint,
		});
		SetRenderableCount(_vertices.size());

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

bool Street::HasSubstreet(const std::shared_ptr<Street>& street)
{
	return std::find(_substreets.begin(), _substreets.end(), street) != _substreets.end();
}

void Street::AddSubstreet(StreetIntersection const& source_intersection, std::shared_ptr<Street> const& substreet)
{
	_substreets.push_back(substreet);
	substreet->parentIntersection = source_intersection;
}

void Street::RemoveSubstreet(const std::shared_ptr<Street>& substreet)
{
	if (!_substreets.empty())
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

void Street::AddIntersection(glm::vec3 const& intersection_point, StreetSegment const& intersecting_segment,
                             StreetSegment const& own_segment)
{
	const auto intersection_side = GetPointSide(intersecting_segment.startPoint, own_segment);
	const auto is_substreet = HasSubstreet(intersecting_segment.street);
	AddIntersection(intersection_point, intersecting_segment, intersection_side, own_segment, is_substreet);
}

void Street::AddIntersection(glm::vec3 const& intersection_point, StreetSegment const& intersecting_segment,
                             const StreetIntersectionSide intersection_side, StreetSegment const& own_segment, const bool is_substreet)
{
	const auto intersecting_street = intersecting_segment.street;
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
			return intersection.intersecting_segment.street == street
				|| intersection.own_segment.street == street;
		}), _intersections.end());
	}
}

void Street::GenerateIntersectionPointLists()
{
	_intersectionPointsLeft.clear();
	_intersectionPointsRight.clear();

	auto source_point_left = GetSegment(0).startPoint;
	auto source_point_right = source_point_left;

	auto last_intersection_left = parentIntersection;
	auto last_intersection_right = parentIntersection;

	for (const auto& intersection : GetIntersections())
	{
		if (intersection.side == LEFT)
		{
			_intersectionPointsLeft.push_back({
				source_point_left,
				intersection.point,
				last_intersection_left,
				intersection,
			});
			source_point_left = intersection.point;
			last_intersection_left = intersection;
		}
		else if (intersection.side == RIGHT)
		{
			_intersectionPointsRight.push_back({
				source_point_right,
				intersection.point,
				last_intersection_right,
				intersection,
			});
			source_point_right = intersection.point;
			last_intersection_right = intersection;
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
	side = GetPointSide(currentPair.point1, currentPair.intersection2.intersecting_segment);
	if (side == LEFT)
	{
		const auto nextPair = std::find_if(_intersectionPointsLeft.begin(), _intersectionPointsLeft.end(),
			[&](StreetNarrowPair const& pair)
		{/*
			if (wasInverted)
				return currentPair.point1 == pair.point2;*/

			return currentPair.point2 == pair.point2;
		});
		if (nextPair != _intersectionPointsLeft.end())
			return *nextPair;
	}
	else if (side == RIGHT)
	{
		const auto nextPair = std::find_if(_intersectionPointsRight.begin(), _intersectionPointsRight.end(),
			[&](StreetNarrowPair const& pair)
		{/*
			if (wasInverted)
				return currentPair.point1 == pair.point1;*/
			return currentPair.point2 == pair.point1;
		});
		if (nextPair != _intersectionPointsRight.end())
			return *nextPair;
	}

	return currentPair;
}

bool Street::GetNextIntersectionPointPair(StreetNarrowPair const& current_pair,
                                          StreetSegment const& intersecting_segment,
                                          const StreetIntersectionSide side_to,
										  StreetNarrowPair* result)
{
	// TODO: Possibly invalide reference segment selection for side evaluation
	const auto side_from = GetPointSide(current_pair.point1, intersecting_segment);
	const auto is_substreet = HasSubstreet(intersecting_segment.street);
	return GetNextIntersectionPointPair(current_pair.point2, side_from, side_to, is_substreet, result);
}

bool Street::GetNextIntersectionPointPair(const glm::vec3& point_from, const StreetIntersectionSide side_from,
                                          const StreetIntersectionSide side_to, const bool is_substreet,
                                          StreetNarrowPair* result)
{
	// pokud křižovatka navazuje zleva a cíl její cesty je doprava
	// dochází k inverzi směru a je nutné zvolit odlišnou dvojici křižovatek
	const auto direction_inverted = side_from != side_to || is_substreet;
	const auto comparer = [&direction_inverted,&point_from](StreetNarrowPair const& pair)
	{
		if (direction_inverted)
			// směr je opačný, hledáme tedy dvojice "z druhé strany"
			return  pair.point2 == point_from;

		// směr nebyl změněn
		return pair.point1 == point_from;
	};

	if (side_from == LEFT)
	{
		const auto result_iter = std::find_if(_intersectionPointsLeft.begin(), _intersectionPointsLeft.end(), comparer);
		if (result_iter == _intersectionPointsLeft.end())
			return false;

		*result = *result_iter;
	}
	else// if (side_to == RIGHT)
	{
		const auto result_iter = std::find_if(_intersectionPointsRight.begin(), _intersectionPointsRight.end(), comparer);
		if (result_iter == _intersectionPointsRight.end())
			return false;

		*result = *result_iter;
	}

	if (direction_inverted/* && !is_substreet*/)
	{
		result->Invert();
	}
	return true;
}
