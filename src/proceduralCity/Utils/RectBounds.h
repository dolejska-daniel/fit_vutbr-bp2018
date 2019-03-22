///
/// @file RectBounds.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


namespace Utils
{
	///
	/// @brief
	///
	class RectBounds
	{
	public:
		///
		/// @brief
		///
		RectBounds(glm::vec2 p, glm::vec2 size);
		///
		/// @brief
		///
		RectBounds(glm::vec3 point1, glm::vec3 point2);
		///
		/// @brief
		///
		~RectBounds();

		///
		/// @brief
		///
		bool Contains(const RectBounds& bounds) const;
		///
		/// @brief
		///
		bool Contains(glm::vec2 p) const;
		///
		/// @brief
		///
		bool IntersectsWith(const RectBounds& bounds) const;

		///
		/// @brief
		///
		bool IsEmpty() const { return size.x == 0.f && size.y == 0.f; }


		///
		/// @brief
		///
		float Top() const { return position.y + size.y; }
		///
		/// @brief
		///
		float Left() const { return position.x; }
		///
		/// @brief
		///
		float Bottom() const { return position.y; }
		///
		/// @brief
		///
		float Right() const { return position.x + size.x; }
		///
		/// @brief
		///
		glm::vec2 LeftBottom() const { return { Left(), Bottom() }; }
		///
		/// @brief
		///
		glm::vec2 RightTop() const { return { Right(), Top() }; }
		///
		/// @brief
		///
		glm::vec2 position{};

		///
		/// @brief
		///
		float Width() const { return size.x; }
		///
		/// @brief
		///
		float Height() const { return size.y; }
		///
		/// @brief
		///
		glm::vec2 size{};
	};
}
