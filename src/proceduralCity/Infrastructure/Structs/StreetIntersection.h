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
		bool					isSubstreet;
		StreetIntersectionSide	side;
		std::shared_ptr<Street>	street;
	};
}
