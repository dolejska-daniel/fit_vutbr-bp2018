///
/// @file BuildingPart.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Infrastructure/BuildingPart.h>
#include <Infrastructure/Parcel.h>


using namespace Infrastructure;

BuildingPart::BuildingPart(const std::shared_ptr<Parcel>& parcel, BuildingType type)
{
	const auto size = parcel->borderPoints.size();
	const auto height_diff = 10.f + rand() % 30;

	std::vector<BuildingPartVertex> vertices;
	for (glm::vec3 point : parcel->borderPoints)
		vertices.push_back({ point });
	for (glm::vec3 point : parcel->borderPoints)
	{
		point.y += height_diff;
		vertices.push_back({ point });
	}

	auto index = [&size](size_t index) -> size_t {
		return index % size;
	};
	std::vector<BuildingPartIndex> indices;
	// stěny
	for (size_t i = 0; i < size; i++)
		indices.push_back({ { i, index(i + 1), size + i }, { size + i, size + index(i + 1), index(i + 1) } });
	// střecha
	indices.push_back({ { size + 0, size + 1, size + 2 }, { size + 0, size + 2, size + 3 } });

	const auto va = CreateVA();
	BindVA();

	const auto vb = CreateVB();
	vb->alloc(vertices.size() * sizeof(BuildingPartVertex), vertices.data());
	va->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(BuildingPartVertex));

	const auto ib = CreateIB();
	ib->alloc(indices.size() * sizeof(BuildingPartIndex), indices.data());
	va->addElementBuffer(ib);
}

BuildingPart::~BuildingPart()
= default;
