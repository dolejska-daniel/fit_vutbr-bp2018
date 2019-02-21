///
/// @file Curve.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <utility>
#include <Utils/Curve.h>


using namespace Utils;

Curve::Curve(std::shared_ptr<std::vector<glm::vec2>> points)
	: _points(std::move(points))
{
}

Curve::~Curve()
= default;

glm::vec2 Curve::GetPoint(const float t) const
{
	auto tmp(*_points);
	int i = _points->size() - 1;
	while (i > 0)
	{
		for (auto k = 0; k < i; k++)
			tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
		i--;
	}
	return tmp[0];
}
