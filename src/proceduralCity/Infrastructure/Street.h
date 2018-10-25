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
		Street(glm::vec3 startPoint, glm::vec3 direction, float length, short level = 0);
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
		inline const glm::vec3 GetSegmentPoint(float t) const {
			auto seg = GetSegment(_segments.size() - 1);
			return (1.f - t) * seg.startPoint + t * seg.endPoint;
		}
		///
		/// @brief 
		///
		inline const glm::vec3 GetSegmentPoint(size_t segment, float t) const {
			auto seg = GetSegment(segment);
			return (1.f - t) * seg.startPoint + t * seg.endPoint;
		}

		///
		/// @brief 
		///
		inline void SetSegmentEndPoint(glm::vec3 endPoint) {
			assert(GetVB() != nullptr);
			_segments.back().endPoint = endPoint;
			_vertices.back().position = endPoint;
			GetVB()->setData(&_vertices[0]);
		}

		///
		/// @brief 
		///
		inline const bool Ended() const { return _ended; }
		///
		/// @brief 
		///
		inline void End() { _ended = true; }

		///
		/// @brief 
		///
		inline const short GetLevel() const { return _level; }

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
