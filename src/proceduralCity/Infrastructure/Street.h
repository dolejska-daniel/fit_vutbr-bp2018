///
/// @file Street.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec3.hpp>
#include <Application/IRenderableArray.h>
#include <Infrastructure/Structs/StreetVertex.h>
#include <Infrastructure/Structs/StreetSegment.h>
#include <Infrastructure/Structs/StreetIntersection.h>
#include <Infrastructure/Structs/StreetNarrowPair.h>
#include "Structs/StreetSegmentIntersection.h"
#include "StreetMap.h"


namespace Terrain
{
	class HeightMap;
}

namespace Infrastructure
{
	class Street;


	///
	/// @brief 
	///
	class Street : public Application::IRenderableArray, public std::enable_shared_from_this<Street>
	{
	public:
		///
		/// @brief 
		///
		Street(Terrain::HeightMap *heightMap, glm::vec3 const& startPoint, glm::vec3 const& direction, float length, short level = 0);
		///
		/// @brief 
		///
		~Street();

		///
		/// @brief 
		///
		void Destroy(const std::shared_ptr<StreetMap>& streetMap );

		///
		/// @brief 
		///
		StreetSegment const& ReadSegment() const;
		///
		/// @brief 
		///
		StreetSegment const& ReadSegment(size_t segment) const;

		///
		/// @brief 
		///
		StreetSegment const& GetSegment();
		///
		/// @brief 
		///
		StreetSegment const& GetSegment(size_t segment);

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
		void SetSegmentEndPoint(size_t segment, glm::vec3 const& endPoint);

		///
		/// @brief 
		///
		void RemoveSegmentsToEnd(size_t segment);
		///
		/// @brief 
		///
		void RebuildVertices();

		///
		/// @brief 
		///
		void ResetSegmentSplit();

		///
		/// @brief 
		///
		bool Destroyed() const { return _destroyed; }
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
		void Unend() { _ended = false; ResetSegmentSplit(); }
		///
		/// @brief 
		///
		void End(glm::vec3 const& intersection_point, StreetSegment const& intersecting_segment, StreetSegment const& own_segment);

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

		bool HasSubstreet(const std::shared_ptr<Street>& street);
		void AddSubstreet(StreetIntersection const& source_intersection, std::shared_ptr<Street> const& substreet);
		void RemoveSubstreet(const std::shared_ptr<Street>& substreet);

		std::vector<StreetIntersection> const& GetIntersections() const;
		StreetIntersectionSide GetPointSide(glm::vec3 const& point, StreetSegment const& segment) const;
		void AddIntersection(glm::vec3 const& intersection_point, StreetSegment const& intersecting_segment, StreetSegment const& own_segment);
		void AddIntersection(glm::vec3 const& intersection_point, StreetSegment const& intersecting_segment, const StreetIntersectionSide
		                     intersection_side, StreetSegment const& own_segment, const bool is_substreet);
		void RemoveIntersection(std::shared_ptr<Street> const& street);

		void GenerateIntersectionPointLists();
		std::vector<StreetNarrowPair> const& GetLeftIntersectionPointPairs() const;
		std::vector<StreetNarrowPair> const& GetRightIntersectionPointPairs() const;

		StreetNarrowPair const& GetNextIntersectionPointPair(StreetNarrowPair const& currentPair, bool wasInverted);
		bool GetNextIntersectionPointPair(StreetNarrowPair const& current_pair, StreetSegment const& intersecting_segment,
		                                  const StreetIntersectionSide side_to, StreetNarrowPair* result);
		bool GetNextIntersectionPointPair(const glm::vec3& point_from, const StreetIntersectionSide side_from,
		                                  const StreetIntersectionSide side_to, const bool is_substreet, StreetNarrowPair* result);

		float		lengthSplit = 0;
		StreetIntersection parentIntersection;

	protected:
		///
		/// @brief 
		///
		Terrain::HeightMap *_heightMap;
		///
		/// @brief 
		///
		std::vector<std::shared_ptr<Street>> _substreets;
		///
		/// @brief 
		///
		std::vector<StreetIntersection> _intersections;
		///
		/// @brief 
		///
		std::vector<StreetNarrowPair> _intersectionPointsLeft;
		///
		/// @brief 
		///
		std::vector<StreetNarrowPair> _intersectionPointsRight;
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
		bool _destroyed = false;
		///
		/// @brief 
		///
		short _level;
	};
}
