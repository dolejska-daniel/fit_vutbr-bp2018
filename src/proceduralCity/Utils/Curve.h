///
/// @file Curve.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


namespace Utils
{
	class Curve2D
	{
	public:
		std::vector<glm::vec2> points;

		Curve2D(std::vector<glm::vec2> points);
		~Curve2D();

		glm::vec2 GetPoint(float t) const;
		glm::vec2 GetPointLinear(float t) const;

	private:
	};

	class Curve3D
	{
	public:
		std::vector<glm::vec3> points;

		Curve3D(std::vector<glm::vec3> points);
		~Curve3D();

		void LaplacianSmooth(int cycles = 1, int precision = 2);

		glm::vec3 GetPoint(float t) const;
		glm::vec3 GetPointLinear(float t) const;
	};
}
