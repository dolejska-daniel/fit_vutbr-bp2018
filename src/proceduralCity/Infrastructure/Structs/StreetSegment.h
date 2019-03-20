///
/// @file StreetSegment.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Utils/IHasRectBounds.h>

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

		///
		/// @brief 
		///
		bool operator==(StreetSegment const& other) const
		{
			return this->startPoint == other.startPoint
				&& this->direction == other.direction;
		}

		///
		/// @brief 
		///
		glm::vec3 GetPoint( const float t ) const
		{
			return (1.f - t) * this->startPoint + t * this->endPoint;
		}
	};

	///
	/// @brief 
	///
	struct StreetSegmentQEntry : Utils::IHasRectBounds
	{
		StreetSegment segment;

		///
		/// @brief 
		///
		StreetSegmentQEntry(const StreetSegment segment)
			: IHasRectBounds(Utils::RectBounds(segment.startPoint, segment.endPoint)),
			segment(segment)
		{
		}
	};
}
