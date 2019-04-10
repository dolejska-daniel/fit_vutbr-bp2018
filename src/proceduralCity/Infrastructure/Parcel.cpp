///
/// @file Parcel.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <Infrastructure/Parcel.h>
#include <Infrastructure/Structs/ParcelVertex.h>
#include <Infrastructure/Structs/ParcelIndex.h>


using namespace Infrastructure;

Parcel::Parcel()
	: type(BUILDING)
{
	SetDrawMode(GL_LINES);
	CreateVA();
	CreateVB();
	CreateIB();
}

Parcel::Parcel(const ParcelType type)
	: type(type)
{
	SetDrawMode(GL_LINES);
	CreateVA();
	CreateVB();
	CreateIB();
}

Parcel::~Parcel()
= default;

bool Parcel::AddBorderPoint(glm::vec3 point)
{
	if (finished) return true;

	point.y += 1;
	//std::cerr << "Parcel border pt.: " << glm::to_string(point) << std::endl;

	if (!borderPoints.empty())
	{
		if (borderPoints.front() == point)
		{
			//std::cerr << "Parcel has been closed by points." << std::endl;
			Finish();
			return false;
		}

		if (borderPoints.back() == point)
		{
			//std::cerr << "Parcel border pt. is same as the last one, skipping." << std::endl;
			return true;
		}
	}

	borderPoints.push_back(point);
	return true;
}

std::vector<glm::vec3> const& Parcel::GetBorderPoints() const
{
	return borderPoints;
}

glm::vec3 Parcel::GetMidpoint()
{
	glm::vec3 midpoint(0);
	for (const auto& point : borderPoints)
		midpoint+= point;
	return midpoint / float(borderPoints.size());
}

void Parcel::Shrink(const float size)
{
	const auto midpoint = GetMidpoint();
	for (auto& point : borderPoints)
		point += glm::normalize(midpoint - point) * size;

	finished = false;
	Finish();
}

void Parcel::Expand(const float size)
{
	const auto midpoint = GetMidpoint();
	for (auto& point : borderPoints)
		point += glm::normalize(point - midpoint) * size;

	finished = false;
	Finish();
}

void Parcel::Finish()
{
	if (finished) return;

	BindVA();

	std::vector<ParcelVertex> vertices;
	for (const auto& point : borderPoints)
		vertices.push_back({ point });

	const auto vb = GetVB();
	vb->alloc(vertices.size() * sizeof(ParcelVertex), vertices.data());
	GetVA()->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(ParcelVertex));

	std::vector<ParcelIndex> indices;
	if (!vertices.empty())
	{
		for (size_t i = 0; i < vertices.size() - 1; i++)
			indices.push_back({ { i, i + 1 } });
		indices.push_back({ { vertices.size() - 1, 0 } });
	}

	const auto ib = GetIB();
	ib->alloc(indices.size() * sizeof(ParcelIndex), indices.data());
	GetVA()->addElementBuffer(ib);

	finished = true;
}
