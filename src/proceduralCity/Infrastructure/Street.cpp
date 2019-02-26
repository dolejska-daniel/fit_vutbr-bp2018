///
/// @file Street.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/glm.hpp>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetNode.h>
#include <Terrain/HeightMap.h>
#include <algorithm>
#include <vector>
#include <glm/detail/_noise.hpp>
#include <glm/detail/_noise.hpp>


using namespace ge::gl;
using namespace Infrastructure;

Street::Street(Terrain::HeightMap* heightMap, glm::vec3 const& startPoint, glm::vec3 const& direction,
               const float length, const short level)
	: _heightMap(heightMap), _level(level)
{
	auto ptr = std::shared_ptr<Street>(this, [](Street*) {});

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
	for (const auto& segment : GetSegments())
		StreetRootNode->Remove(segment);

	if (parentStreet)
		parentStreet->RemoveSubstreet(this->shared_from_this());

	// TODO: Napojit segmenty
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
		//	Směrnice nejsou stejné, přidáváme nový segment a vertexy
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

void Street::AddSubstreet(std::shared_ptr<Street> const& substreet)
{
	_substreets.push_back(substreet);
	substreet->parentStreet = this->shared_from_this();
}

void Street::RemoveSubstreet(const std::shared_ptr<Street>& substreet)
{
	_substreets.erase(std::remove(_substreets.begin(), _substreets.end(), substreet), _substreets.end());
}

std::vector<StreetIntersection> const& Street::GetIntersections() const
{
	return _intersections;
}

void Street::AddIntersection(glm::vec3 const& intersection_point, StreetSegment const& intersecting_segment, StreetSegment const& segment)
{
	const auto a = segment.startPoint;
	const auto b = segment.endPoint;
	const auto p = intersecting_segment.startPoint;

	//--------------------------------------------------------------------
	// https://math.stackexchange.com/a/274728
	//
	const auto d = (p.x - a.x) * (b.z - a.z) - (p.z - a.z) * (b.x - a.x);
	//--------------------------------------------------------------------

	StreetIntersectionSide side;
	if (d < 0)
		side = RIGHT;
	else
		side = LEFT;

	AddIntersection(intersection_point, segment.street, side);
}

void Street::AddIntersection(glm::vec3 const& intersection_point, std::shared_ptr<Street> const& street,
                             const StreetIntersectionSide side)
{
	const auto is_substreet = std::find(_substreets.begin(), _substreets.end(), street) != _substreets.end();
	_intersections.push_back({
		intersection_point,
		is_substreet,
		side,
		street,
	});

	std::sort(_intersections.begin(), _intersections.end(),
		[&](StreetIntersection const& intersection1, StreetIntersection const& intersection2)
	{
		const auto start_point = GetSegment(0).startPoint;
		return glm::distance(start_point, intersection1.point) < glm::distance(start_point, intersection2.point);
	});
}

void Street::RemoveIntersection(std::shared_ptr<Street> const& street)
{
	_intersections.erase(std::remove_if(_intersections.begin(), _intersections.end(),
		[&](StreetIntersection const& intersection)
	{
		return intersection.street == street;
	}), _intersections.end());
}
