///
/// @file StreetZone.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/exponential.hpp>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetMap.h>
#include <Infrastructure/StreetZone.h>


using namespace Infrastructure;


const float stepSize = 4.f;
const float stepLevelOffset = .8f;
const float splitLimit = 24.f;

StreetZone::StreetZone(vars::Vars& vars, glm::vec2 const& center, float const radius):
	_vars(vars), _center(center), _radius(radius)
{
}

StreetZone::~StreetZone()
= default;


glm::vec3 const& StreetZone::GetDirection(std::shared_ptr<Street> const& street)
{
	return street->ReadSegment().direction;
}

float StreetZone::GetLength(std::shared_ptr<Street> const& street)
{
	return glm::pow(stepLevelOffset, street->GetLevel()) * stepSize;
}

void StreetZone::BuildStep(std::shared_ptr<Street> const& street)
{
	//	TODO: Nalezení odpovídající zóny ze seznamu
	street->BuildStep(GetDirection(street), GetLength(street));
}

void StreetZone::SplitStep(StreetMap* map, std::shared_ptr<Street> const& street)
{
	const auto segment = street->GetSegment();
	if (segment.lengthSplit >= splitLimit && street->GetLevel() < 3)
	{
		street->ResetSegmentSplit();

		glm::vec3 position = street->GetSegmentPoint(1.f - ((std::rand() % 12) / 100.f));
		glm::vec3 direction;

		if (std::rand() % 10 <= 5)
		{
			direction.x = -segment.direction.z;
			direction.y = segment.direction.y;
			direction.z = segment.direction.x;
		}
		else
		{
			direction.x = segment.direction.z;
			direction.y = segment.direction.y;
			direction.z = -segment.direction.x;
		}

		map->GetStreets().push_back(std::make_shared<Street>(position, direction, GetLength(street), street->GetLevel() + 1));
	}
}
