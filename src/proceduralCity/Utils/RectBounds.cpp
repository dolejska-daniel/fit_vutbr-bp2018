///
/// @file RectBounds.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <vector>
#include <algorithm>
#include <Utils/RectBounds.h>
#include <glm/glm.hpp>

using namespace Utils;


RectBounds::RectBounds(const glm::vec2 p, const glm::vec2 size)
	: position(p), size(size)
{
}

RectBounds::RectBounds(const glm::vec3 point1, const glm::vec3 point2)
{
	std::vector<glm::vec3> points{ point1, point2 };
	const auto x_extremes = std::minmax_element(points.begin(), points.end(), [](const glm::vec3& p1, const glm::vec3& p2)
	{
		return p1.x < p2.x;
	});
	const auto z_extremes = std::minmax_element(points.begin(), points.end(), [](const glm::vec3& p1, const glm::vec3& p2)
	{
		return p1.z < p2.z;
	});

	position = {x_extremes.first->x, z_extremes.first->z};
	const auto dir = glm::abs(point2 - point1);
	size = { dir.x, dir.z };
}

RectBounds::~RectBounds()
= default;

bool RectBounds::Contains(const RectBounds& bounds) const
{
	return Contains(bounds.LeftBottom())
		&& Contains(bounds.RightTop());
}

bool RectBounds::Contains(const glm::vec2 p) const
{
	return p.x >= Left()
		&& p.x <= Right()
		&& p.y >= Bottom()
		&& p.y <= Top();
}

bool RectBounds::IntersectsWith(const RectBounds& bounds) const
{
	return bounds.Left() <= Right()
		&& bounds.Right() >= Left()
		&& bounds.Bottom() <= Top()
		&& bounds.Top() >= Bottom();
}
