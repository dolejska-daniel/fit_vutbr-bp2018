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
	template<typename T>
	static void MergeVectors(std::vector<T>& vec1, const std::vector<T>& vec2)
	{
		vec1.insert(std::end(vec1), std::begin(vec2), std::end(vec2));
	}
}
