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
#include <glm/glm.hpp>
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

	static std::vector<glm::vec3> create_block_base(const std::vector<glm::vec3>& p, const float padding = 0.f, const float height = 0.f, float al = 0.f, float bl = 0.f)
	{
		if (al == 0.f)
			al = glm::length(p[1] - p[0]) * (1.f - padding);
		if (bl == 0.f)
			bl = glm::length(p[3] - p[0]) * (1.f - padding);
		const auto vl = glm::length(p[2] - p[0]);

		const auto a = glm::normalize(p[1] - p[0]);
		const auto b = glm::normalize(p[3] - p[0]);
		const auto v = glm::normalize(a * al + b * bl);
		const auto o = v * padding / 2.f * vl;

		std::vector<glm::vec3> points
		{
			p[0] + o,
			p[0] + o + a * al,
			p[0] + o + v * sqrt(al * al + bl * bl),
			p[0] + o + b * bl,
		};
		if (height != 0)
		{
			for (auto& point : points)
				point.y = height;
		}

		return points;
	}

	static std::vector<glm::vec3> move_vecs(const std::vector<glm::vec3>& v, const glm::vec3& dir, float len = 1)
	{
		auto o(v);
		for (auto& vec : o)
			vec += dir * len;
		return o;
	}

	static glm::vec3 random_dir_vec()
	{
		auto dir_rand = rand() % 2000 / 1000.f;
		dir_rand -= 1;

		return glm::normalize(glm::vec3{ glm::cos(dir_rand), 0, glm::sin(dir_rand) });
	}

	static float randomf(float from = 0.f, float to = 1.f)
	{
		from *= 1000.f;
		to *= 1000.f;
		auto r = rand() % int(to - from) / 1000.f;
		r += from;

		return r;
	}

	static float randomi(int from = 0, int to = 1)
	{
		to += 1;
		auto r = rand() % (to - from) / 1000;
		r += from;
		r -= 1;

		return r;
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

	const glm::vec3 color_black = { 0, 0, 0 };

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
