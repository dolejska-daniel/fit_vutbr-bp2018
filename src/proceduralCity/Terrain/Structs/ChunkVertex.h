///
/// @file ChunkVertex.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec3.hpp>

namespace Terrain
{
	///
	/// @brief
	///
	struct ChunkVertex
	{
		glm::vec3 position;		///< Poziční vektor
		glm::vec3 normal;		///< Normálový vektor
		glm::vec3 textureMix;	///< 
	};
}
