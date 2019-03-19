///
/// @file StreetMap.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec2.hpp>
#include <Application/IRenderableCollection.h>
#include <Infrastructure/Structs/StreetSegmentIntersection.h>


namespace vars
{
	class Vars;
}

namespace Terrain
{
	class Map;
}

namespace Infrastructure
{
	class Street;
	class StreetNode;
	class StreetZone;


	///
	/// @brief 
	///
	class StreetMap : public std::enable_shared_from_this<StreetMap>
	{
	public:
		Terrain::Map *terrainMap;

		///
		/// @brief 
		///
		StreetMap(Terrain::Map *map);
		///
		/// @brief 
		///
		~StreetMap();

		///
		/// @brief 
		///
		std::vector<std::shared_ptr<Street>> &GetStreets() { return _streets; }
		///
		/// @brief 
		///
		std::vector<std::shared_ptr<Street>> ReadStreets() const { return _streets; }
		///
		/// @brief 
		///
		void AddStreet(const std::shared_ptr<Street>& street);
		void RemoveStreet(const std::shared_ptr<Street>& street);


		///
		/// @brief 
		///
		static StreetSegmentIntersection Intersection(StreetSegment const& segment, std::shared_ptr<Street> const& street);
		///
		/// @brief 
		///
		static StreetSegmentIntersection Intersection(StreetSegment const& segment1, StreetSegment const& segment2);
		///
		/// @brief 
		///
		std::shared_ptr<std::vector<StreetSegmentIntersection>> Intersections(StreetSegment const& segment) const;
		///
		/// @brief 
		///
		void Intersections(StreetSegment const& segment, std::shared_ptr<StreetNode> const& node, std::shared_ptr<std::vector<StreetSegmentIntersection>> const& intersections) const;

		///
		/// @brief 
		///
		void ValidateIntersections();
		///
		/// @brief 
		///
		void ValidateIntersections(const std::shared_ptr<Street>& street);

		///
		/// @brief 
		///
		void BuildStep();

	private:
		std::vector<std::shared_ptr<Street>> _streets;

		std::shared_ptr<StreetZone> _zone;
	};
}
