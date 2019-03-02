///
/// @file BuildingPart.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Application/IRenderableElementArray.h>
#include <Infrastructure/Structs/BuildingPartVertex.h>
#include <Infrastructure/Structs/BuildingPartIndex.h>


namespace Infrastructure
{
	class BuildingPart : Application::IRenderableElementArray
	{
	public:
		BuildingPart();
		~BuildingPart();
	};
}
