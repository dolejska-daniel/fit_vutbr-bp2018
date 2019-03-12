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
	struct StreetIntersection;


	///
	/// @brief 
	///
	struct StreetNarrowPair
	{
		glm::vec3			point1;
		glm::vec3			point2;
		StreetIntersection	intersection1;
		StreetIntersection	intersection2;

		bool operator==(StreetNarrowPair const& other) const
		{
			return this->point1 == other.point1
				&& this->point2 == other.point2
				&& this->intersection1.side == other.intersection1.side
				&& this->intersection2.side == other.intersection2.side;
		}

		void Invert()
		{
			std::swap(this->point1, this->point2);
			std::swap(this->intersection1, this->intersection2);
			this->intersection1.Invert();
			this->intersection2.Invert();
		}
	};
}
