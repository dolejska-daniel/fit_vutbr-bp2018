///
/// @file StreetIntersection.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

namespace Infrastructure
{
	class Street;


	///
	/// @brief 
	///
	enum StreetIntersectionSide { LEFT, RIGHT, };

	///
	/// @brief 
	///
	struct StreetIntersection
	{
		glm::vec3				point;
		StreetIntersectionSide	side;
		bool					is_substreet;
		StreetSegment			intersecting_segment;
		StreetSegment			own_segment;

		bool operator==(StreetIntersection const& other) const
		{
			return this->point == other.point
				&& this->side == other.side
				&& this->is_substreet == other.is_substreet
				&& this->intersecting_segment == other.intersecting_segment
				&& this->own_segment == other.own_segment;
		}

		void Invert()
		{
			std::swap(this->own_segment, this->intersecting_segment);
		}
	};
}
