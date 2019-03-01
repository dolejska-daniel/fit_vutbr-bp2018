///
/// @file StreetNarrowPair.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

namespace Infrastructure
{
	class Street;

	struct StreetSegment;
	struct StreetSegmentIntersection;


	///
	/// @brief 
	///
	struct StreetNarrowPair
	{
		glm::vec3		point1;
		glm::vec3		point2;
		StreetSegment	intersecting_segment1;
		StreetSegment	intersecting_segment2;
		bool			invertedNext;

		bool operator==(StreetNarrowPair const& other) const
		{
			return this->point1 == other.point1
				&& this->point2 == other.point2
				&& this->intersecting_segment1.street == other.intersecting_segment1.street
				&& this->intersecting_segment2.street == other.intersecting_segment2.street
				&& this->invertedNext == other.invertedNext;
		}
	};
}
