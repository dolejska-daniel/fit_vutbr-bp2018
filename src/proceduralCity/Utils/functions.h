///
/// @file Curve.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <fstream>
#include <vector>
#include <memory>
#include <string>


namespace Utils
{
	template<typename T>
	static void merge_vectors(std::vector<T>& vec1, const std::vector<T>& vec2)
	{
		vec1.insert(std::end(vec1), std::begin(vec2), std::end(vec2));
	}

	static glm::vec3 vec4to3(const glm::vec4& v)
	{
		return { v.x, v.y, v.z };
	}

	static glm::vec2 vec3to2(const glm::vec3& v)
	{
		return { v.x, v.z };
	}

	static glm::vec3 vec2to3(const glm::vec2& v)
	{
		return { v.x, 0, v.y };
	}

	static glm::vec4 vec3to4(const glm::vec3& v)
	{
		return { v.x, v.y, v.z, 0 };
	}

	const glm::vec3 color_white = { 1, 1, 1 };

	const glm::vec3 color_red = { 1, 0, 0 };

	const glm::vec3 color_green = { 0, 1, 0 };

	const glm::vec3 color_blue = { 0, 0, 1 };

	static glm::vec3 color_rgb(const unsigned r, const unsigned g, const unsigned b)
	{
		return {
			r * (1.f / 255.f),
			g * (1.f / 255.f),
			b * (1.f / 255.f),
		};
	}

	static glm::vec4 color_rgba(const unsigned r, const unsigned g, const unsigned b, const float a)
	{
		return {
			r * (1.f / 255.f),
			g * (1.f / 255.f),
			b * (1.f / 255.f),
			a,
		};
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

	static std::ifstream read_file(const std::string& filepath)
	{
		return std::ifstream(filepath.c_str());
	}

	static std::ofstream write_file(const std::string& filepath)
	{
		return std::ofstream(filepath.c_str());
	}

	static std::fstream open_file(const std::string& filepath)
	{
		return std::fstream(filepath.c_str());
	}
}
