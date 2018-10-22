///
/// @file Street.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Infrastructure/Street.h>


using namespace ge::gl;
using namespace Infrastructure;

Street::Street(glm::vec3 startPoint, glm::vec3 direction, float length)
{
	SetDrawMode(GL_LINES);

	auto va = CreateVA();
	BindVA();

	auto vb = CreateVB();
	BindVB();

	_segments.push_back({
		startPoint,
		startPoint + length * direction,
		direction,
		length,
	});

	vb->alloc(2 * sizeof(StreetVertex));
	Build();
	va->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(StreetVertex));
}

Street::~Street()
{
}


void Street::Build()
{
	assert(GetVB() != nullptr);
	//	TODO: Kontrola intersectu s jinou ulicí

	StreetSegment newSegment = {
		GetSegment().endPoint,
		GetSegment().endPoint + GetSegment().length * GetSegment().direction,
		GetSegment().direction,
		GetSegment().length,
	};
	if (newSegment.direction == GetSegment().direction && _vertices.size() && _segments.size())
	{
		//	Směrnice jsou stejné, pouze prodloužíme původní úsečku

		_segments.back().endPoint = newSegment.endPoint;
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
			//	Zdvojnásobení aktuální velikosti bufferu
			GetVB()->realloc(GetVB()->getSize() * 2, Buffer::ReallocFlags::KEEP_ID);
	}

	//	Aktualizace dat v bufferu
	GetVB()->setData(&_vertices[0], _vertices.size() * sizeof(StreetVertex));
}


/*

auto s1 = GetSegment();
for (size_t i = 0; i < street->GetSegmentCount(); i++)
{
	//	Segment ulice
	auto s2 = street->GetSegment();
	if (s1.direction == s2.direction)
		//	Směrnice jsou stejné, průsečík neexistuje
		continue;

	//s1.
}

*/

