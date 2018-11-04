///
/// @file StreetMap.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Application/IRenderableCollection.h>
#include <Infrastructure/Street.h>


namespace Infrastructure
{
	///
	/// @brief 
	///
	struct StreetSegmentIntersection
	{
		bool		exists;
		glm::vec2	positionRelative;
	};


	///
	/// @brief 
	///
	class StreetMap
	{
	public:
		///
		/// @brief 
		///
		StreetMap();
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
		std::vector<StreetSegmentIntersection> IntersectionsWithAny(StreetSegment const& segment) const;

		///
		/// @brief 
		///
		void BuildStep();

	private:
		std::vector<std::shared_ptr<Street>> _streets;
	};
}
