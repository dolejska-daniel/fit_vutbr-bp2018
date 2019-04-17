///
/// @file BuildingPart.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Infrastructure/BuildingPart.h>
#include <Infrastructure/Parcel.h>
#include <Utils/functions.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>


using namespace Infrastructure;

BuildingPart::BuildingPart(Terrain::HeightMap* heightMap, const std::vector<glm::vec3>& borderPoints,
                           BuildingType type)
{
	std::cerr << "Setting up building part." << std::endl;

	auto points = borderPoints;
	for (auto& point : points)
		point.y = heightMap->GetData(point);

	auto height = points.front().y;
	glm::vec3 center(0);
	for (const auto& point : points)
	{
		center += point;
		if (point.y > height)
			height = point.y;
	}
	center /= float(points.size());
	height += .1f;

	const auto padding = .7f;
	const auto noise_change = 10.f;
	const auto noise_scale = 10.f;
	const auto noise_coeff = 2.f;

	const auto size = points.size();
	const auto main_height_diff = 10.f + rand() % 20;
	const auto noise = glm::perlin(center / noise_scale) * noise_coeff;

	std::cerr << noise << std::endl;

	const auto a   = glm::normalize(points[1] - points[0]);
	const auto al  = glm::length(points[1] - points[0]) * (1.f - padding); // a length
	const auto ali = glm::length(points[1] - points[0]) * padding * .5f; // a length inversed

	const auto b   = glm::normalize(points[3] - points[0]);
	const auto bl  = glm::length(points[3] - points[0]) * (1.f - padding); // b length
	const auto bli = glm::length(points[3] - points[0]) * padding * .5f; // b length inversed

	const auto minli = glm::min(ali, bli);

	std::vector<BuildingPartVertex> vertices;

	// základ
	CreateBlock(points, vertices, height);

	// hlavní blok
	auto height_main = height + 10.f + rand() % 400 / 20.f;
	auto base_points = Utils::create_block_base(points, .4f + rand() % 20 / 100.f, height);
	CreateBlock(base_points, vertices, height_main, height);

	// další bloky
	auto i_max = 2 + rand() % 3;
	for (auto i = 0; i < i_max; i++)
	{
		auto dir_rand = rand() % 2000 / 1000.f;
		dir_rand -= 1;

		auto size_rand = rand() % 1200 / 1000.f;
		size_rand -= .2f;

		//auto x = glm::perlin(center);
		//std::cerr << x << std::endl;

		float height_rand = float(rand() % 400);
		height_rand /= 20;
		height_rand += 5.f;
		height_rand += glm::pow(noise_change, noise);
		
		auto dir = glm::normalize(glm::vec3{ glm::cos(dir_rand), 0, glm::sin(dir_rand) });
		base_points = Utils::create_block_base(points, padding + rand() % 100 / 1000.f - .05f, height);
		CreateBlock(Utils::move_vecs(base_points, dir, minli * size_rand), vertices, height + height_rand, height);
	}

	/*
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
	}*/

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

void BuildingPart::CreateBlock(const std::vector<glm::vec3>& points, std::vector<BuildingPartVertex>& vertices, float height_top, float height_bottom)
{
	glm::vec3 center(0);
	glm::vec3 center_up(0);
	for (auto& point : points)
		center += point;
	center_up = center /= points.size();
	center_up.y = height_top;

	if (height_bottom != 0)
		center.y = height_bottom;

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
		if (height_bottom != 0)
		{
			p0.y = height_bottom;
			p1.y = height_bottom;
		}

		auto p2 = get_point_h(index + 0, height_top);
		auto p3 = get_point_h(index + 1, height_top);
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
		vertices.push_back({ get_point_h(index + 0, height_top), up });
		vertices.push_back({ get_point_h(index + 1, height_top), up });
		vertices.push_back({ center_up, up });
	}
}
