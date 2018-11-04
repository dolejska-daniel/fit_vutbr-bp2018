///
/// @file Street.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/glm.hpp>
#include <Infrastructure/Street.h>


using namespace ge::gl;
using namespace Infrastructure;

Street::Street(glm::vec3 const& startPoint, glm::vec3 const& direction, const float length, const short level)
	: _level(level)
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
		length,
	};
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


StreetSegment Street::GetSegment() const
{
	assert(!_segments.empty());
	return _segments[_segments.size() - 1];
}

StreetSegment Street::GetSegment(const size_t segment) const
{
	assert(segment >= 0);
	assert(segment < _segments.size());
	return _segments[segment];
}


glm::vec3 Street::GetSegmentPoint(const float t) const
{
	const auto seg = GetSegment(_segments.size() - 1);
	return (1.f - t) * seg.startPoint + t * seg.endPoint;
}

glm::vec3 Street::GetSegmentPoint(const size_t segment, const float t) const
{
	const auto seg = GetSegment(segment);
	return (1.f - t) * seg.startPoint + t * seg.endPoint;
}

void Street::SetSegmentEndPoint(glm::vec3 const& endPoint)
{
	assert(GetVB() != nullptr);
	_segments.back().endPoint = endPoint;
	_vertices.back().position = endPoint;
	GetVB()->setData(&_vertices[0]);
}

void Street::BuildStep()
{
	BuildStep(GetSegment().direction, GetSegment().length);
}

void Street::BuildStep(glm::vec3 const& direction)
{
	BuildStep(direction, GetSegment().length);
}

void Infrastructure::Street::BuildStep(const float length)
{
	BuildStep(GetSegment().direction, length);
}

void Street::BuildStep(glm::vec3 const& direction, const float length)
{
	assert(GetVB() != nullptr);

	const StreetSegment newSegment {
		GetSegment().endPoint,
		GetSegment().endPoint + length * direction,
		direction,
		length,
		0
	};
	if (newSegment.direction == GetSegment().direction && !_vertices.empty() && !_segments.empty())
	{
		//	Směrnice jsou stejné, pouze prodloužíme původní úsečku
		_segments.back().endPoint = newSegment.endPoint;
		_segments.back().length+= length;
		_vertices.back().position = newSegment.endPoint;
	}
	else
	{
		//	Směrnice nejsou stejné, přidáváme nový segment a vertexy

		//	Uložení nového segmentu
		_segments.push_back(newSegment);
		//	Uložení nových vertexů
		_vertices.push_back({
			newSegment.startPoint,
		});
		_vertices.push_back({
			newSegment.endPoint,
		});

		//	Kontrola velikosti bufferu
		if (_vertices.size() * sizeof(StreetVertex) > GetVB()->getSize())
		{
			//	Zdvojnásobení aktuální velikosti bufferu
			GetVB()->realloc(GetVB()->getSize() * 2, Buffer::ReallocFlags::KEEP_ID);
		}
	}

	//	Aktualizace dat v bufferu
	GetVB()->setData(&_vertices[0], _vertices.size() * sizeof(StreetVertex));
}
