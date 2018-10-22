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
	struct StreetVertex
	{
		glm::vec3 position;
	};

	struct StreetSegment
	{
		glm::vec3	startPoint;
		glm::vec3	endPoint;
		glm::vec3	direction;
		float		length;
	};

	class Street : public Application::IRenderableArray
	{
	public:
		Street(glm::vec3 startPoint, glm::vec3 direction, float length);
		~Street();

		inline StreetSegment GetSegment(int segment = -1) const {
			int segment_id = segment == -1 ? _segments.size() - 1 : segment;
			assert(segment_id >= 0);
			assert(segment_id < _segments.size());
			return _segments[segment_id];
		}
		inline size_t GetSegmentCount() const { return _segments.size(); }

		inline glm::vec3 GetStartPoint(int segment = -1) const { return GetSegment(segment).startPoint; }
		inline glm::vec3 GetDirection(int segment = -1) const { return GetSegment(segment).direction; }
		inline float GetLength(int segment = -1) const { return GetSegment(segment).length; }

		void Build();

	private:
		std::vector<StreetVertex> _vertices;
		std::vector<StreetSegment> _segments;
	};
}
