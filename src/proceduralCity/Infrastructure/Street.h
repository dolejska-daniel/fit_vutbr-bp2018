///
/// @file Street.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec3.hpp>
#include <Application/IRenderableArray.h>


namespace Infrastructure
{
	///
	/// @brief 
	///
	struct StreetVertex
	{
		glm::vec3 position;
	};

	///
	/// @brief 
	///
	struct StreetSegment
	{
		glm::vec3	startPoint;
		glm::vec3	endPoint;
		glm::vec3	direction;
		float		length;
		int			lastSplit;

		bool operator==(StreetSegment const& other) const
		{
			return this->startPoint == other.startPoint
				&& this->direction == other.direction;
		}
	};


	///
	/// @brief 
	///
	class Street : public Application::IRenderableArray
	{
	public:
		///
		/// @brief 
		///
		Street(glm::vec3 const& startPoint, glm::vec3 const& direction, float length, short level = 0);
		///
		/// @brief 
		///
		~Street();

		///
		/// @brief 
		///
		StreetSegment GetSegment() const;
		///
		/// @brief 
		///
		StreetSegment GetSegment(const size_t segment) const;

		///
		/// @brief 
		///
		std::vector<StreetSegment> GetSegments() const { return _segments; }
		///
		/// @brief 
		///
		glm::vec3 GetSegmentPoint(float t) const;
		///
		/// @brief 
		///
		glm::vec3 GetSegmentPoint(size_t segment, float t) const;

		///
		/// @brief 
		///
		void SetSegmentEndPoint(glm::vec3 const& endPoint);

		///
		/// @brief 
		///
		bool Ended() const { return _ended; }
		///
		/// @brief 
		///
		void End() { _ended = true; }

		///
		/// @brief 
		///
		short GetLevel() const { return _level; }

		///
		/// @brief 
		///
		void BuildStep();
		///
		/// @brief 
		///
		void BuildStep(glm::vec3 const& direction);
		///
		/// @brief 
		///
		void BuildStep(float length);
		///
		/// @brief 
		///
		void BuildStep(glm::vec3 const& direction, float length);

	protected:
		///
		/// @brief 
		///
		std::vector<StreetVertex> _vertices;
		///
		/// @brief 
		///
		std::vector<StreetSegment> _segments;

		///
		/// @brief 
		///
		bool _ended = false;
		///
		/// @brief 
		///
		short _level;
	};
}
