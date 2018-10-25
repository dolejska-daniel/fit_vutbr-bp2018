///
/// @file Street.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/glm.hpp>
#include <Infrastructure/Street.h>


using namespace ge::gl;
using namespace Infrastructure;

Street::Street(glm::vec3 startPoint, glm::vec3 direction, float length, short level)
	: _level(level)
{
	SetDrawMode(GL_LINES);

	auto va = CreateVA();
	BindVA();

	auto vb = CreateVB();
	BindVB();

	StreetSegment newSegment = {
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
{
}


void Street::BuildStep()
{
	BuildStep(GetSegment().direction, GetSegment().length);
}

void Street::BuildStep(glm::vec3 direction)
{
	BuildStep(direction, GetSegment().length);
}

void Infrastructure::Street::BuildStep(float length)
{
	BuildStep(GetSegment().direction, length);
}

void Street::BuildStep(glm::vec3 direction, float length)
{
	assert(GetVB() != nullptr);

	//	TODO: Udržovat informace o minX, minZ, maxX a maxZ (určuje obdélník, ve kterém se celá ulice nachází)
	//	TODO: Předvypočítat determinant
	StreetSegment newSegment = {
		GetSegment().endPoint,
		GetSegment().endPoint + length * direction,
		direction,
		length,
	};
	if (newSegment.direction == GetSegment().direction && _vertices.size() && _segments.size())
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
