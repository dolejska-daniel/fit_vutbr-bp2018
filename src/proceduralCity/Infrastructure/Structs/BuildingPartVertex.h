///
/// @file BuildingPartVertex.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Infrastructure
{
	///
	/// @brief 
	///
	struct BuildingPartVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoord;
		short	  textureSlot;
	};
}
