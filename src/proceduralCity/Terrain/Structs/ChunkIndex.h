///
/// @file ChunkIndex.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

namespace Terrain
{
	///
	/// @brief
	///
	struct ChunkIndex
	{
		glm::uvec3 triangle1;	///< Tři vrcholy prvního trojúhelníka segmentu
		glm::uvec3 triangle2;	///< Tři vrcholy druhého trojúhelníka segmentu
	};
}
