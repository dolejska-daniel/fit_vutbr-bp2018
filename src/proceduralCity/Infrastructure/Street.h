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
		Street(glm::vec3 startPoint, glm::vec3 direction, float length);
		///
		/// @brief 
		///
		~Street();

		///
		/// @brief 
		///
		inline const StreetSegment GetSegment() const {
			assert(_segments.size() > 0);
			return _segments[_segments.size() - 1];
		}
		///
		/// @brief 
		///
		inline const StreetSegment GetSegment(size_t segment) const {
			assert(segment >= 0);
			assert(segment < _segments.size());
			return _segments[segment];
		}
		///
		/// @brief 
		///
		inline const std::vector<StreetSegment> GetSegments() const { return _segments; }
		///
		/// @brief 
		///
		inline const size_t GetSegmentCount() const { return _segments.size(); }

		///
		/// @brief 
		///
		inline const float Ended() const { return _ended; }
		///
		/// @brief 
		///
		inline void End() { _ended = true; }

		///
		/// @brief 
		///
		void BuildStep();
		///
		/// @brief 
		///
		void BuildStep(glm::vec3 direction);
		///
		/// @brief 
		///
		void BuildStep(float length);
		///
		/// @brief 
		///
		void BuildStep(glm::vec3 direction, float length);

	private:
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
	};
}
