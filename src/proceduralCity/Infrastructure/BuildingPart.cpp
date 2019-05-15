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
#include <Application/Application.h>


using namespace glm;
using namespace Infrastructure;

BuildingPart::BuildingPart(Terrain::HeightMap* heightMap, const std::vector<glm::vec3>& borderPoints,
                           BuildingType type): _heightMap(heightMap), textureTint(glm::vec3(1))
{
	//std::cerr << "Setting up building part." << std::endl;
	auto randomTint = Utils::randomi(0, 10);
	if (randomTint <= 3);
	else if (randomTint <= 6)
		textureTint = Utils::create_random_orange_tint();
	else if (randomTint <= 9)
		textureTint = Utils::create_random_blue_tint();
	else
		textureTint = Utils::create_random_black_tint();

	if (type == SQUARE)
		RandomBuildingSquareDefault(borderPoints, vertices);
	else if (type == STREET_SQUARE)
		StreetSquare(borderPoints, vertices);

	const auto va = CreateVA();
	BindVA();

	const auto vb = CreateVB();
	SetRenderableCount(vertices.size());
	vb->alloc(vertices.size() * sizeof(BuildingPartVertex), vertices.data());
	va->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(BuildingPartVertex), 0 * sizeof(vec3));
	va->addAttrib(vb, 1, 3, GL_FLOAT, sizeof(BuildingPartVertex), 1 * sizeof(vec3));
	va->addAttrib(vb, 2, 3, GL_FLOAT, sizeof(BuildingPartVertex), 2 * sizeof(vec3));
	va->addAttrib(vb, 3, 2, GL_FLOAT, sizeof(BuildingPartVertex), 3 * sizeof(vec3));
}

BuildingPart::~BuildingPart()
= default;

void BuildingPart::StreetSquare(const std::vector<glm::vec3>& borderPoints, std::vector<BuildingPartVertex>& vertices)
{
	auto points = borderPoints;
	for (auto& point : points)
		point.y = 0.f;

	CreateBlock(points, vertices, 1.f, 0.f);
	for (auto& vertex : vertices)
	{
		if (vertex.position.y == 1.f)
			vertex.position.y = _heightMap->GetData(vertex.position) + .25f;
	}
}

void BuildingPart::RandomBuildingSquareDefault(const std::vector<glm::vec3>& borderPoints, std::vector<BuildingPartVertex>& vertices)
{
	auto points = borderPoints;
	for (auto& point : points)
		point.y = _heightMap->GetData(point);

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

	const auto padding = .6f;
	const auto noise_change = Application::Vars.getFloat("buildings.noise.change");
	const auto noise_scale = Application::Vars.getFloat("buildings.noise.scale");
	const auto noise_coeff = Application::Vars.getFloat("buildings.noise.coeff");

	const auto size = points.size();
	auto noise = glm::perlin(center / noise_scale);
	noise *= noise_coeff;

	const auto a = glm::normalize(points[1] - points[0]);
	const auto al = glm::length(points[1] - points[0]) * (1.f - padding); // a length
	const auto ali = glm::length(points[1] - points[0]) * padding * .4f; // a length inversed

	const auto b = glm::normalize(points[3] - points[0]);
	const auto bl = glm::length(points[3] - points[0]) * (1.f - padding); // b length
	const auto bli = glm::length(points[3] - points[0]) * padding * .4f; // b length inversed

	const auto minli = min(ali, bli);

	// nastavení 
	auto height_main = height + 10.f + rand() % 400 / 30.f;
	height_main += glm::pow(noise_change, noise);

	auto dir = Utils::random_dir_vec();

	auto size_rand = rand() % 1200 / 1000.f;
	size_rand -= .2f;

	// základy
	CreateBlock(points, vertices, height, 0, BASE);

	// hlavní blok
	auto base_points = Utils::create_block_base(points, .3f + rand() % 20 / 100.f, height);
	CreateBlock(Utils::move_vecs(base_points, dir, minli * size_rand), vertices, height_main, height, MAIN);

	// střešní bloky
	auto top_points = Utils::create_block_base(base_points, .1f + (rand() % 14 - 8) / 100.f, height_main);
	top_points = Utils::move_vecs(top_points, dir, minli * size_rand);

	auto top_blocks_min = Application::Vars.getUint32("buildings.blocks.top.min");
	auto top_blocks_max = Application::Vars.getUint32("buildings.blocks.top.max");

	const auto height_core = height_main;
	auto t_max = Utils::randomi(top_blocks_min, top_blocks_max);
	for (auto t = 0; t < t_max; t++)
	{
		auto top_height_diff = height_main * ((200 + rand() % 300) / 10000.f);
		if (top_height_diff > 20.f)
			top_height_diff = 20.f;

		auto top_height = height_main + top_height_diff;
		CreateBlock(top_points, vertices, top_height, height_main, TOP);

		height_main = top_height;
		top_points = Utils::create_block_base(top_points, .1f + (rand() % 14 - 8) / 100.f, height_main);
	}

	auto blocks_min = Application::Vars.getUint32("buildings.blocks.min");
	auto blocks_max = Application::Vars.getUint32("buildings.blocks.max");

	// další bloky
	auto i_max = Utils::randomi(blocks_min, blocks_max);
	for (auto i = 0; i < i_max; i++)
	{
		dir = Utils::random_dir_vec();

		size_rand = rand() % 1200 / 1000.f;
		size_rand -= .2f;

		auto height_rand = .2f + rand() % 700 / 1000.f;

		base_points = Utils::create_block_base(points, padding + rand() % 100 / 1000.f - .05f, height);
		CreateBlock(Utils::move_vecs(base_points, dir, minli * size_rand), vertices, height_core * height_rand, height, PART);
	}
}

void BuildingPart::CreateBlock(const std::vector<glm::vec3>& points, std::vector<BuildingPartVertex>& vertices, float height_top, float height_bottom, BlockType type)
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
		if (glm::dot(n, nm) > 0)
			n = glm::normalize(glm::cross(n2, n3));
		if (glm::dot(n, nm) > 0)
			n = glm::normalize(glm::cross(n3, n1));

		auto textureScaleFactor = float(Application::Vars.getUint32("buildings.windows.scale"));
		if (type == BASE)
			textureScaleFactor = 0;

		auto textureScaleRatio = vec2{ 1.f / 4.f, 1.f / 3.f };
		auto textureScale = vec2{ length(n1), length(n2) } * textureScaleRatio * textureScaleFactor;
		textureScale = floor(textureScale);

		vertices.push_back({ p0, n, textureTint, vec2{ 0.f, 0.f } * textureScale });
		vertices.push_back({ p1, n, textureTint, vec2{ 1.f, 0.f } * textureScale });
		vertices.push_back({ p2, n, textureTint, vec2{ 0.f, 1.f } * textureScale });

		vertices.push_back({ p1, n, textureTint, vec2{ 1.f, 0.f } * textureScale });
		vertices.push_back({ p2, n, textureTint, vec2{ 0.f, 1.f } * textureScale });
		vertices.push_back({ p3, n, textureTint, vec2{ 1.f, 1.f } * textureScale });
	}
	const auto up = glm::vec3(0, 1.f, 0);
	for (size_t index = 0; index < points.size(); ++index)
	{
		vertices.push_back({ get_point_h(index + 0, height_top), up, textureTint, {0, 0} });
		vertices.push_back({ get_point_h(index + 1, height_top), up, textureTint, {0, 0} });
		vertices.push_back({ center_up, up, textureTint, { 0, 0 } });
	}
}
