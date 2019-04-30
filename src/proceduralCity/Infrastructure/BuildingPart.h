///
/// @file BuildingPart.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Application/IRenderableArray.h>
#include <Infrastructure/Structs/BuildingPartVertex.h>
#include <Infrastructure/Building.h>
#include <Terrain/HeightMap.h>


namespace Terrain {
	class HeightMap;
}

namespace Infrastructure
{
	class Parcel;

	class BuildingPart : public Application::IRenderableArray
	{
	public:
		BuildingPart(Terrain::HeightMap* heightMap, const std::vector<glm::vec3>& borderPoints, BuildingType type);
		~BuildingPart();

		void StreetSquare(const std::vector<glm::vec3>& borderPoints, std::vector<BuildingPartVertex>& vertices);
		void RandomBuildingSquareDefault(const std::vector<glm::vec3>& borderPoints, std::vector<BuildingPartVertex>& vertices);
		void CreateBlock(const std::vector<glm::vec3>& points, std::vector<BuildingPartVertex>& vertices, float height_top, float height_bottom = 0);
		Terrain::HeightMap* _heightMap;
		std::vector<BuildingPartVertex> vertices;
	};
}
