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


using namespace Infrastructure;

BuildingPart::BuildingPart(const std::shared_ptr<Parcel>& parcel, BuildingType type)
{
	std::cerr << "Setting up building part." << std::endl;

	const auto points = parcel->borderPoints;
	const auto size = points.size();
	const auto height_diff = 5.f + rand() % 20;

	glm::vec3 center(0);
	glm::vec3 center_up(0);
	for (auto point : points)
		center+= point;
	center_up = center/= size;
	center_up.y+= height_diff;

	const auto get_point = [&size,&points,&height_diff](const size_t index, const bool top = false) -> glm::vec3 {
		auto point = points[index % size];
		if (top) point.y+= height_diff;
		return point;
	};
	std::vector<BuildingPartVertex> vertices;
	for (size_t index = 0; index < size; ++index)
	{
		auto p0 = get_point(index + 0);
		auto p1 = get_point(index + 1);
		auto p2 = get_point(index + 0, true);
		auto p3 = get_point(index + 1, true);
		auto m = (p0 + p1) / 2.f;

		auto nm = glm::normalize(center - m);
		auto n1 = p1 - p0;
		auto n2 = p2 - p0;
		auto n3 = p3 - p0;

		auto n = glm::normalize(glm::cross(n1, n2));
		if (nm.x * n.x >= 0.f && nm.z * n.z >= 0.f)
			n = glm::normalize(glm::cross(n2, n3));

		vertices.push_back({ p0, n });
		vertices.push_back({ p1, n });
		vertices.push_back({ p2, n });

		vertices.push_back({ p1, n });
		vertices.push_back({ p2, n });
		vertices.push_back({ p3, n });
	}
	const auto up = glm::vec3(0, 1.f, 0);
	for (size_t index = 0; index < size; ++index)
	{
		vertices.push_back({ get_point(index + 0, true), up });
		vertices.push_back({ get_point(index + 1, true), up });
		vertices.push_back({ center_up, up });
	}


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
