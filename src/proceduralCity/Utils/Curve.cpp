///
/// @file Curve.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <utility>
#include <Utils/Curve.h>


using namespace Utils;

Curve2D::Curve2D(std::vector<glm::vec2> points)
	: points(std::move(points))
{
}

Curve2D::~Curve2D()
= default;

glm::vec2 Curve2D::GetPoint(const float t) const
{
	auto tmp(points);
	int i = points.size() - 1;
	while (i > 0)
	{
		for (auto k = 0; k < i; k++)
			tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
		i--;
	}
	return tmp[0];
}

glm::vec2 Curve2D::GetPointLinear(const float t) const
{
	if (points.size() < 2)
		return { 0.f, 0.f };

	const auto part = 1.f / points.size();
	unsigned index = floor(t / part);
	if (index >= points.size())
		index = points.size() - 1;

	auto p1 = points.at(index);
	auto p2 = points.at(index + 1);

	auto v = 1.f - part / (t - index * part);
	return p1 * v
		+ p2 * (1.f - v);
}


Curve3D::Curve3D(std::vector<glm::vec3> points)
	: points(std::move(points))
{
}

Curve3D::~Curve3D()
= default;

void Curve3D::LaplacianSmooth(const int cycles, const int precision)
{
	std::vector<glm::vec3> pts;
	for (auto point : points)
	{
		pts.push_back(point);
	}
	for (auto i = 0; i < points.size() - 1; ++i)
	{
		auto p1 = points[i];
		auto p2 = points[i + 1];
		auto d = p2 - p1;

		pts.push_back(p1);
		for (int j = 1; j <= precision; ++j)
		{
			pts.push_back(p1 + d * float(j * 1.f / (precision + 1)));
		}
	}
	pts.push_back(points.at(points.size() - 1));

	for (auto j = 0; j < cycles; ++j)
	{
		for (auto i = 1; i < pts.size() - 1; ++i)
			pts[i] = .5f * (pts.at(i - 1) + pts.at(i + 1));
	}

	points = pts;
}

glm::vec3 Curve3D::GetPoint(const float t) const
{
	auto tmp(points);
	int i = points.size() - 1;
	while (i > 0)
	{
		for (auto k = 0; k < i; k++)
			tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
		i--;
	}
	return tmp[0];
}

glm::vec3 Curve3D::GetPointLinear(const float t) const
{
	if (points.size() < 2)
		return { 0.f, 0.f, 0.f };

	const auto part = 1.f / points.size();
	unsigned index = floor(t / part);
	if (index >= points.size())
		index = points.size() - 1;

	auto p1 = points.at(index);
	auto p2 = points.at(index + 1);
	auto d = p2 - p1;

	auto v = 1.f - part / (t - index * part);
	return p1 * v
		+ d * (1.f - v);
}

