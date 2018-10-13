///
/// @file HeightMap.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

#include <glm/glm.hpp>
#include <Vars/Vars.h>

#include "Fwd.h"


namespace Terrain
{
	///
	/// @brief
	///
	class HeightMap
	{
	public:
		HeightMap(vars::Vars& vars, Chunk* chunk);
		~HeightMap();

		void Populate();

		float GetData(int x, int y);
		inline float GetData(glm::vec2 v) { return GetData(v.x, v.y); }
		inline float GetData(glm::vec3 v) { return GetData(v.x, v.z); }

		unsigned int GetWidth();
		unsigned int GetHeight();

		inline float *GetMap() { return map; }

	private:
		vars::Vars &vars;
		Chunk* chunk;

		float *map; ///< Data vıškové mapy, 2D float pole

		inline int GetMapElementCount() { return GetMapElementCount(GetWidth(), GetHeight()); }
		inline int GetMapElementCount(unsigned int width, unsigned int height) { return width * height; }

		inline int GetMapIndex(int x, int y) { return y * GetWidth() + x; }
	};
}
