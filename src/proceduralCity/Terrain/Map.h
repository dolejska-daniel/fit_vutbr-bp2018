///
/// @file Map.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Vars/Fwd.h>
#include <Terrain/Fwd.h>


namespace Terrain
{
	///
	/// @brief
	///
	class Map
	{
	public:
		///
		/// @brief
		///
		Map(vars::Vars& vars);
		///
		/// @brief
		///
		~Map();

		///
		/// @brief
		///
		vars::Vars& GetVars() const { return _vars; }
		///
		/// @brief
		///
		HeightMap* GetHeightMap() const { return _heightMap; }
		///
		/// @brief
		///
		Chunk** GetChunks() const { return _chunks; }

	private:
		vars::Vars& _vars;	///<

		HeightMap* _heightMap;	///<
		Chunk** _chunks;		///<
	};
}
