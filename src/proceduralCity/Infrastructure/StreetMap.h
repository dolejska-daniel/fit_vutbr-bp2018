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
		glm::vec3	position;
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
		inline std::vector<std::shared_ptr<Street>> &GetStreets() { return _streets; }
		///
		/// @brief 
		///
		inline const std::vector<std::shared_ptr<Street>> ReadStreets() const { return _streets; }

		///
		/// @brief 
		///
		StreetSegmentIntersection Intersection(const StreetSegment segment, const std::shared_ptr<Street> street) const;
		///
		/// @brief 
		///
		std::vector<StreetSegmentIntersection> IntersectionsWithAny(const StreetSegment segment) const;

		///
		/// @brief 
		///
		void BuildStep();

	private:
		std::vector<std::shared_ptr<Street>> _streets;
	};
}
