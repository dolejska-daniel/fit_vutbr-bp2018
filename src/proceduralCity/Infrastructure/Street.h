///
/// @file Street.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec3.hpp>
#include <Application/IRenderableArray.h>


namespace Terrain
{
	class HeightMap;
}

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

		bool operator==(StreetSegment const& other) const
		{
			return this->startPoint == other.startPoint
				&& this->direction == other.direction;
		}
	};


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
		void ResetSegmentSplit();

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

		void AddSubstreet(std::shared_ptr<Street> const& substreet);
		void RemoveSubstreet(const std::shared_ptr<Street>& substreet);

		void SetParentStreet(std::shared_ptr<Street> const& parent_street);

		float		lengthSplit = 0;
		std::shared_ptr<Street> parentStreet;

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
