///
/// @file Parcel.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/detail/_noise.hpp>
#include <Infrastructure/Parcel.h>
#include <Infrastructure/Structs/ParcelVertex.h>
#include <Infrastructure/Structs/ParcelIndex.h>


using namespace Infrastructure;

Parcel::Parcel()
	: finished(false)
{
	CreateVA();
	CreateVB();
	CreateIB();
}

Parcel::~Parcel()
= default;

void Parcel::AddBorderPoint(glm::vec3 const& point)
{
	_borderPoints.push_back(point);
}

void Parcel::Finish()
{
	BindVA();

	std::vector<ParcelVertex> vertices;
	for (const auto& point : _borderPoints)
		vertices.push_back({ point });

	const auto vb = GetVB();
	vb->alloc(vertices.size() * sizeof(ParcelVertex), vertices.data());
	GetVA()->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(ParcelVertex));


	std::vector<ParcelIndex> indices;
	for (size_t i = 1; i < vertices.size() - 1; i++)
		indices.push_back({ { i, i + 1, 0 } });

	const auto ib = GetIB();
	ib->alloc(indices.size() * sizeof(ParcelIndex), indices.data());
	GetVA()->addElementBuffer(ib);

	finished = true;
}
