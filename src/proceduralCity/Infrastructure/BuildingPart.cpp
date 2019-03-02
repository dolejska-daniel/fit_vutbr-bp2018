///
/// @file BuildingPart.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Infrastructure/BuildingPart.h>


using namespace Infrastructure;

BuildingPart::BuildingPart()
{
	std::vector<BuildingPartVertex> vertices;
	std::vector<BuildingPartIndex> indices;
	/*
	for (size_t i = 0; i < vertices.size() - 1; i++)
		indices.push_back({ { 0, 0, 0 }, { 0, 0, 0 } });
	*/

	const auto va = CreateVA();
	BindVA();

	const auto vb = CreateVB();
	vb->alloc(vertices.size() * sizeof(BuildingPartVertex), vertices.data());
	va->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(BuildingPartVertex));

	const auto ib = CreateIB();
	ib->alloc(indices.size() * sizeof(BuildingPartIndex), indices.data());
	va->addElementBuffer(ib);
}

BuildingPart::~BuildingPart()
= default;
