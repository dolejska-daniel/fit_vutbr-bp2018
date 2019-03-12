///
/// @file BuildingPart.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Application/IRenderableElementArray.h>
#include <Infrastructure/Structs/BuildingPartVertex.h>
#include <Infrastructure/Structs/BuildingPartIndex.h>
#include <Infrastructure/Building.h>


namespace Infrastructure
{
	class Parcel;

	class BuildingPart : public Application::IRenderableElementArray
	{
	public:
		BuildingPart(const std::shared_ptr<Parcel>& parcel, BuildingType type);
		~BuildingPart();
	};
}
