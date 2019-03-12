///
/// @file Building.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Infrastructure/Building.h>
#include <Infrastructure/BuildingPart.h>

using namespace Infrastructure;


Building::Building(std::shared_ptr<Parcel> parcel, const BuildingType type)
	: _parcel(std::move(parcel)), _type(type)
{
	GenerateParts();
}

Building::~Building()
= default;

void Building::GenerateParts()
{
	if (_type == SQUARE)
		GenerateParts_Square();
}

void Building::GenerateParts_Square()
{
	const auto part = std::make_shared<BuildingPart>(_parcel, _type);
	parts.push_back(part);
}
