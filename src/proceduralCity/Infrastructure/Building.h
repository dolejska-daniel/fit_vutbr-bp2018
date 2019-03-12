///
/// @file Building.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <vector>
#include <memory>


namespace Infrastructure
{
	class Parcel;
	class BuildingPart;

	enum BuildingType
	{
		SQUARE
	};

	class Building 
	{
	public:

		std::vector<std::shared_ptr<BuildingPart>> parts;

		Building(std::shared_ptr<Parcel> parcel, BuildingType type);
		~Building();

		void GenerateParts();
		void GenerateParts_Square();

	protected:
		std::shared_ptr<Parcel> _parcel;
		BuildingType _type;
	};
}
