///
/// @file BuildingPart.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Infrastructure/BuildingPart.h>
#include <Infrastructure/Parcel.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/detail/_noise.hpp>
#include <vector>
#include <glm/detail/_noise.hpp>
#include <vector>
#include <glm/detail/_noise.hpp>
#include <vector>


using namespace Infrastructure;

BuildingPart::BuildingPart(const std::vector<glm::vec3>& borderPoints, BuildingType type)
{
	std::cerr << "Setting up building part." << std::endl;

	auto points = borderPoints;
	auto height = points.front().y;
	glm::vec3 center(0);
	for (const auto& point : points)
	{
		center += point;
		if (point.y > height)
			height = point.y;
	}
	center /= float(points.size());
	const auto size = points.size();
	const auto main_height_diff = 10.f + rand() % 20;

	std::vector<BuildingPartVertex> vertices;
	CreateBlock(points, vertices, height + .1f);

	for (auto& point : points)
		point += glm::normalize(center - point) * 6.f;
	CreateBlock(points, vertices, height + main_height_diff);
	const auto old_points = points;

	auto block_count = 2 + rand() % 2;
	auto top_blocks_height = main_height_diff;
	for (auto i = 0; i < block_count; i++)
	{
		auto dir_size = float(1.f + rand() % 3);
		for (auto& point : points)
			point += glm::normalize(center - point) * dir_size;
		top_blocks_height += 1.f + rand() % 4;
		CreateBlock(points, vertices, height + top_blocks_height);
	}

	points = old_points;
	block_count = 1 + rand() % 3;
	for (auto i = 0; i < block_count; i++)
	{
		auto dir = glm::normalize(glm::vec3{ -50.f + rand() % 101, 0.f, -50.f + rand() % 101 });
		auto dir_size = 2.f + float(rand() % 2);
		auto shrink_size = 2.f + float(rand() % 4);
		for (auto& point : points)
		{
			point += glm::normalize(center - point) * dir_size;
			point += dir * shrink_size;
		}
		CreateBlock(points, vertices, height + 6.f + rand() % 15);
		points = old_points;
	}
	/**/

	const auto va = CreateVA();
	BindVA();

	const auto vb = CreateVB();
	SetRenderableCount(vertices.size());
	vb->alloc(vertices.size() * sizeof(BuildingPartVertex), vertices.data());
	va->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(BuildingPartVertex), 0 * sizeof(glm::vec3));
	va->addAttrib(vb, 1, 3, GL_FLOAT, sizeof(BuildingPartVertex), 1 * sizeof(glm::vec3));
}

BuildingPart::~BuildingPart()
= default;

void BuildingPart::CreateBlock(const std::vector<glm::vec3>& points, std::vector<BuildingPartVertex>& vertices, float height)
{
	glm::vec3 center(0);
	glm::vec3 center_up(0);
	for (auto& point : points)
		center += point;
	center_up = center /= points.size();
	center_up.y = height;

	const auto get_point = [&points](const size_t index) -> glm::vec3 {
		auto point = points[index % points.size()];
		return point;
	};
	const auto get_point_h = [&points](const size_t index, const float height) -> glm::vec3 {
		auto point = points[index % points.size()];
		point.y = height;
		return point;
	};

	for (size_t index = 0; index < points.size(); ++index)
	{
		auto p0 = get_point(index + 0);
		auto p1 = get_point(index + 1);
		auto p2 = get_point_h(index + 0, height);
		auto p3 = get_point_h(index + 1, height);
		auto m = (p0 + p1) / 2.f;

		auto nm = glm::normalize(center - m);
		auto n1 = p1 - p0;
		auto n2 = p2 - p0;
		auto n3 = p3 - p0;

		auto n = glm::normalize(glm::cross(n1, n2));
		if (nm.x * n.x >= 0.f && nm.z * n.z >= 0.f)
			n = glm::normalize(glm::cross(n2, n3));
		if (nm.x * n.x >= 0.f && nm.z * n.z >= 0.f)
			n = glm::normalize(glm::cross(n1, n3));

		vertices.push_back({ p0, n });
		vertices.push_back({ p1, n });
		vertices.push_back({ p2, n });

		vertices.push_back({ p1, n });
		vertices.push_back({ p2, n });
		vertices.push_back({ p3, n });
	}
	const auto up = glm::vec3(0, 1.f, 0);
	for (size_t index = 0; index < points.size(); ++index)
	{
		vertices.push_back({ get_point_h(index + 0, height), up });
		vertices.push_back({ get_point_h(index + 1, height), up });
		vertices.push_back({ center_up, up });
	}
}
