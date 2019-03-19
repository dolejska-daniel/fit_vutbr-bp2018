///
/// @file Parcel.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <Infrastructure/Parcel.h>
#include <Infrastructure/Structs/ParcelVertex.h>
#include <Infrastructure/Structs/ParcelIndex.h>
#include <glm/detail/_noise.hpp>
#include <glm/detail/_noise.hpp>


using namespace Infrastructure;

Parcel::Parcel()
	: finished(false)
{
	SetDrawMode(GL_LINES);
	CreateVA();
	CreateVB();
	CreateIB();
}

Parcel::~Parcel()
= default;

bool Parcel::AddBorderPoint(glm::vec3 point)
{
	if (finished) return true;

	point.y += 1;
	//std::cerr << "Parcel border pt.: " << glm::to_string(point) << std::endl;

	if (!borderPoints.empty())
	{
		if (borderPoints.front() == point)
		{
			Finish();
			return false;
		}

		if (borderPoints.back() == point)
		{
			//std::cerr << "Parcel border pt. is same as the last one, skipping." << std::endl;
			return true;
		}
	}

	/*
	if (!borderPoints.empty())
		std::cerr << "Front  border pt.: " << glm::to_string(borderPoints.front()) << std::endl;
	if (!borderPoints.empty() && borderPoints.front() == point)
	{
		std::cerr << "Parcel has been closed by points." << std::endl;
		return Finish();
	}*/
	borderPoints.push_back(point);
	return true;
}

std::vector<glm::vec3> const& Parcel::GetBorderPoints() const
{
	return borderPoints;
}

void Parcel::Finish()
{
	if (finished) return;

	BindVA();

	std::vector<ParcelVertex> vertices;
	for (const auto& point : borderPoints)
		vertices.push_back({ point });

	const auto vb = GetVB();
	vb->alloc(vertices.size() * sizeof(ParcelVertex), vertices.data());
	GetVA()->addAttrib(vb, 0, 3, GL_FLOAT, sizeof(ParcelVertex));

	std::vector<ParcelIndex> indices;
	if (!vertices.empty())
	{
		for (size_t i = 0; i < vertices.size() - 1; i++)
			indices.push_back({ { i, i + 1 } });
		indices.push_back({ { vertices.size() - 1, 0 } });
	}

	const auto ib = GetIB();
	ib->alloc(indices.size() * sizeof(ParcelIndex), indices.data());
	GetVA()->addElementBuffer(ib);

	finished = true;
}
