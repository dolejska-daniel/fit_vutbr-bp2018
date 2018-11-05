///
/// @file Map.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <memory>
#include <map>
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
		unsigned int GetHeight() const { return _height; }
		///
		/// @brief
		///
		unsigned int GetWidth() const { return _width; }

		///
		/// @brief
		///
		unsigned int GetChunkHeight() const { return _chunkHeight; }
		///
		/// @brief
		///
		unsigned int GetChunkWidth() const { return _chunkWidth; }

		///
		/// @brief
		///
		HeightMap* GetHeightMap() const { return _heightMap; }

		///
		/// @brief
		///
		std::map<unsigned int, std::shared_ptr<Chunk>> &GetChunks() { return _chunks; }
		///
		/// @brief
		///
		std::shared_ptr<Chunk> &GetChunk(unsigned int x, unsigned int y);
		///
		/// @brief
		///
		std::shared_ptr<Chunk> ReadChunk(unsigned int x, unsigned int y) const;

	private:
		vars::Vars& _vars;	///<

		unsigned int _width;	///<
		unsigned int _height;	///<

		unsigned int _chunkWidth;	///<
		unsigned int _chunkHeight;	///<

		HeightMap* _heightMap;	///<
		std::map<unsigned int, std::shared_ptr<Chunk>> _chunks;		///<
	};
}
