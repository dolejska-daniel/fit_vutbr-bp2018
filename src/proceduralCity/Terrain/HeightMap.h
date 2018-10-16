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
	/// @class HeightMap
	/// @brief
	///
	class HeightMap
	{
	public:
		HeightMap(vars::Vars& vars);
		~HeightMap();

		float GetData(float x, float y, unsigned int detail);
		inline float GetData(glm::vec2 v, unsigned int detail) { return GetData(v.x, v.y, detail); }
		inline float GetData(glm::vec3 v, unsigned int detail) { return GetData(v.x, v.z, detail); }

	private:
		vars::Vars &vars;
	};
}
