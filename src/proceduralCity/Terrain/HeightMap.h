///
/// @file HeightMap.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <Vars/Vars.h>
#include <Terrain/Fwd.h>


namespace Terrain
{
	///
	/// @brief
	///
	class HeightMap
	{
	public:
		///
		/// @brief
		///
		HeightMap(vars::Vars& vars);
		///
		/// @brief
		///
		~HeightMap();

		///
		/// @brief
		///
		float GetData(float x, float y, unsigned int detail) const;
		///
		/// @brief
		///
		float GetData(glm::vec2 const& v, const unsigned int detail);

		///
		/// @brief
		///
		float GetData(glm::vec3 const& v, const unsigned int detail);

	private:
		vars::Vars& _vars;	///< 
	};
}
