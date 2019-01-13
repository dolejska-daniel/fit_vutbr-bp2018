///
/// @file StreetMap.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Application/IRenderableCollection.h>
#include <glm/vec2.hpp>


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
	struct StreetSegmentIntersection
	{
		bool		exists;
		glm::vec2	positionRelative;

		bool operator==(StreetSegmentIntersection const& other) const
		{
			return this->exists == other.exists
				&& this->positionRelative == other.positionRelative;
		}
	};


	///
	/// @brief 
	///
	class StreetMap
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
		void BuildStep();

	private:
		std::vector<std::shared_ptr<Street>> _streets;

		std::shared_ptr<StreetZone> _zone;
	};
}
