///
/// @file StreetSegment.h
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
	struct StreetSegment
	{
		glm::vec3	startPoint;
		glm::vec3	endPoint;
		glm::vec3	direction;
		float		length;
		std::shared_ptr<Street>	street;

		bool operator==(StreetSegment const& other) const
		{
			return this->startPoint == other.startPoint
				&& this->direction == other.direction;
		}
	};
}
