///
/// @file StreetZone.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/exponential.hpp>
#include <utility>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetMap.h>
#include <Infrastructure/StreetZone.h>
#include <glm/detail/func_geometric.inl>


using namespace Infrastructure;


const float stepSize = 4.f;
const float stepLevelOffset = .8f;
const float splitLimit = 24.f;

StreetZone::StreetZone(vars::Vars& vars, glm::vec2 const& center, const float radius):
	_vars(vars), _center(center), _radius(radius)
{
	_buildStep = [=](std::shared_ptr<Street> const& street)
	{
		auto dir = street->ReadSegment().direction;
		
		street->BuildStep(dir, glm::pow(stepLevelOffset, street->GetLevel()) * stepSize);
	};

	_splitStep = [=](StreetMap* map, std::shared_ptr<Street> const& street)
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

			map->GetStreets().push_back(std::make_shared<Street>(position, direction, glm::pow(stepLevelOffset, street->GetLevel()) * stepSize, street->GetLevel() + 1));
		}
	};
}

StreetZone::StreetZone(vars::Vars& vars, glm::vec2 const& center, const float radius,
	std::function<void(std::shared_ptr<Street> const& street)> buildStep,
	std::function<void(StreetMap* map, std::shared_ptr<Street> const& street)> splitStep):
	_vars(vars), _center(center), _radius(radius), _buildStep(std::move(buildStep)), _splitStep(std::move(splitStep))
{
}

StreetZone::~StreetZone()
= default;


void StreetZone::BuildStep(std::shared_ptr<Street> const& street)
{
	//	TODO: Nalezení odpovídající zóny ze seznamu
	_buildStep(street);
}

void StreetZone::SplitStep(StreetMap* map, std::shared_ptr<Street> const& street)
{
	//	TODO: Nalezení odpovídající zóny ze seznamu
	_splitStep(map, street);
}
