///
/// @file Curve.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <memory>


namespace Utils
{
	template<typename T>
	static void merge_vectors(std::vector<T>& vec1, const std::vector<T>& vec2)
	{
		vec1.insert(std::end(vec1), std::begin(vec2), std::end(vec2));
	}

	static glm::vec3 right_vector(const glm::vec3& vec)
	{
		return {
			-vec.z,
			vec.y,
			vec.x,
		};
	}

	static glm::vec3 left_vector(const glm::vec3& vec)
	{
		return {
			vec.z,
			vec.y,
			-vec.x,
		};
	}
}
