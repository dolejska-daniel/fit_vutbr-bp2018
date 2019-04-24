///
/// @file Building.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <vector>
#include <memory>
#include <Terrain/HeightMap.h>


namespace Infrastructure
{
	class Parcel;
	class BuildingPart;

	enum BuildingType
	{
		SQUARE,
		STREET_SQUARE
	};

	class Building 
	{
	public:

		std::vector<std::shared_ptr<BuildingPart>> parts;

		Building(std::shared_ptr<Parcel> parcel, Terrain::HeightMap *heightMap, BuildingType type);
		~Building();

		void GenerateParts();
		void GenerateParts_Square();
		void GenerateParts_StreetSquare();

	protected:
		std::shared_ptr<Parcel> _parcel;
		Terrain::HeightMap* _heightMap;
		BuildingType _type;
	};
}
