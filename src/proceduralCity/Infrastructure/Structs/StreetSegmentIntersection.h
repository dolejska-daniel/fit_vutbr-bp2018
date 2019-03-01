///
/// @file StreetSegmentIntersection.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec2.hpp>

namespace Infrastructure
{
	struct StreetSegment;

	///
	/// @brief 
	///
	struct StreetSegmentIntersection
	{
		bool			exists;
		glm::vec2		positionRelative;
		StreetSegment	intersectingSegment;
		StreetSegment	ownSegment;

		bool operator==(StreetSegmentIntersection const& other) const
		{
			return this->exists == other.exists
				&& this->positionRelative == other.positionRelative;
		}
	};
}
