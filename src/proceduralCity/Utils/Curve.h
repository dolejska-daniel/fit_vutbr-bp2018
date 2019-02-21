///
/// @file Curve.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec2.hpp>
#include <vector>
#include <memory>


namespace Utils
{
	class Curve
	{
	public:
		Curve(std::shared_ptr<std::vector<glm::vec2>> points);
		~Curve();

		glm::vec2 GetPoint(float t) const;

	private:
		std::shared_ptr<std::vector<glm::vec2>> _points;
	};
}
