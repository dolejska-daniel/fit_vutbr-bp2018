///
/// @file StreetZone.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/exponential.hpp>
#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/random.hpp>
#include <utility>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetMap.h>
#include <Infrastructure/StreetZone.h>
#include <Terrain/Map.h>


using namespace Infrastructure;


const float stepSize = 6.f;
const float stepLevelOffset = .75f;
const float splitLimit = 24.f;

StreetZone::StreetZone(vars::Vars& vars, glm::vec2 const& center, const float radius,
	std::function<void(std::shared_ptr<Street> const& street)> buildStep,
	std::function<void(StreetMap* map, std::shared_ptr<Street> const& street)> splitStep):
	_vars(vars), _center(center), _radius(radius), _buildStep(std::move(buildStep)), _splitStep(std::move(splitStep))
{
	if (_buildStep == nullptr)
	{
		_buildStep = [&](std::shared_ptr<Street> const& street)
		{
			auto dir = street->ReadSegment().direction;
			street->BuildStep(dir, glm::pow(stepLevelOffset, street->GetLevel()) * stepSize);
		};
	}

	if (_splitStep == nullptr)
	{
		_splitStep = [&](StreetMap* map, std::shared_ptr<Street> const& street)
		{
			const auto segment = street->GetSegment();
			if (street->lengthSplit >= splitLimit && street->GetLevel() < 4)
			{
				street->ResetSegmentSplit();

				glm::vec3 position = street->GetSegmentPoint(1.f - ((25 + std::rand() % 50) / 100.f));
				glm::vec3 direction;
				if (std::rand() % 10 >= 5)
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

				const auto substreet = std::make_shared<Street>(map->terrainMap->GetHeightMap(), position, direction, glm::pow(stepLevelOffset, street->GetLevel()) * stepSize, street->GetLevel() + 1);
				street->AddSubstreet(substreet);
				map->AddStreet(substreet);
			}
		};
	}
}

StreetZone::~StreetZone()
= default;


bool StreetZone::Add(std::shared_ptr<StreetZone> const& zone)
{
	if (glm::distance(_center, zone->GetCenter()) + zone->GetRadius() < _radius)
	{
		_zones.emplace_back(zone);
		return true;
	}

	for (auto z = _zones.rbegin(); z != _zones.rend(); ++z)
		if ((*z)->Add(zone))
			return true;

	return false;
}


bool StreetZone::BuildStep(std::shared_ptr<Street> const& street)
{
	if (glm::distance(street->ReadSegment().endPoint, glm::vec3(_center.x, street->ReadSegment().endPoint.y, _center.y)) > _radius)
		return false;

	for (auto z = _zones.rbegin(); z != _zones.rend(); ++z)
		if ((*z)->BuildStep(street))
			return true;

	_buildStep(street);
	return true;
}

bool StreetZone::SplitStep(StreetMap* map, std::shared_ptr<Street> const& street)
{
	if (glm::distance(street->ReadSegment().endPoint, glm::vec3(_center.x, street->ReadSegment().endPoint.y, _center.y)) > _radius)
		return false;

	for (auto z = _zones.rbegin(); z != _zones.rend(); ++z)
		if ((*z)->BuildStep(street))
			return true;

	_splitStep(map, street);
	return true;
}
