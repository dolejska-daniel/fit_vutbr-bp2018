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
	: finished(false)
{
	SetDrawMode(GL_LINES);
	CreateVA();
	CreateVB();
	CreateIB();
}

Parcel::~Parcel()
= default;

void Parcel::AddBorderPoint(glm::vec3 point)
{
	if (finished) return;

	point.y += 1;
	std::cerr << "Parcel border pt.: " << glm::to_string(point) << std::endl;
	/*
	if (!_borderPoints.empty())
		std::cerr << "Front  border pt.: " << glm::to_string(_borderPoints.front()) << std::endl;
	if (!_borderPoints.empty() && _borderPoints.front() == point)
	{
		std::cerr << "Parcel has been closed by points." << std::endl;
		return Finish();
	}*/
	_borderPoints.push_back(point);
}

void Parcel::Finish()
{
	if (finished) return;

	BindVA();

	std::vector<ParcelVertex> vertices;
	for (const auto& point : _borderPoints)
		vertices.push_back({ point });

	const auto vb = GetVB();
	vb->alloc(vertices.size() * sizeof(ParcelVertex), vertices.data());
	GetVA()->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(ParcelVertex));

	std::vector<ParcelIndex> indices;
	for (size_t i = 0; i < vertices.size() - 1; i++)
		indices.push_back({ { i, i + 1 } });
	indices.push_back({ { vertices.size() - 1, 0 } });

	const auto ib = GetIB();
	ib->alloc(indices.size() * sizeof(ParcelIndex), indices.data());
	GetVA()->addElementBuffer(ib);

	finished = true;
}
