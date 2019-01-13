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


using namespace ge::gl;
using namespace Infrastructure;

Street::Street(Terrain::HeightMap* heightMap, glm::vec3 const& startPoint, glm::vec3 const& direction,
               const float length, const short level)
	: _heightMap(heightMap), _level(level)
{
	SetDrawMode(GL_LINES);

	auto va = CreateVA();
	BindVA();

	auto vb = CreateVB();
	BindVB();

	const StreetSegment newSegment = {
		startPoint,
		startPoint + length * direction,
		direction,
		length
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
= default;


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
			length
		};

		newSegment.endPoint.y = _heightMap->GetData(newSegment.endPoint);

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
		});/*
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
