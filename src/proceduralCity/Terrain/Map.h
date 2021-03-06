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
#include <glm/vec2.hpp>


namespace Infrastructure {
	class Street;
}

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
		Map();
		///
		/// @brief
		///
		~Map();


		int GetChunkOffsetX(const int x) const {
			const auto globalX = -(static_cast<int>(_chunkWidth) / 2);
			return globalX + x * _chunkWidth * 8;
		};

		int GetChunkOffsetY(const int y) const {
			const auto globalY = -(static_cast<int>(_chunkHeight) / 2);
			return globalY + y * _chunkHeight * 8;
		};

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
		std::shared_ptr<Chunk> &GetChunk(unsigned int index);

		///
		/// @brief
		///
		bool ValidateStreet(std::shared_ptr<Infrastructure::Street> const& street) const;

	private:
		unsigned int _width;	///<
		unsigned int _height;	///<

		unsigned int _chunkWidth;	///<
		unsigned int _chunkHeight;	///<

		glm::vec2 _offsetMin{0, 0};	///<
		glm::vec2 _offsetMax{0, 0};	///<

		HeightMap* _heightMap;	///<
		std::map<unsigned int, std::shared_ptr<Chunk>> _chunks;		///<
	};
}
