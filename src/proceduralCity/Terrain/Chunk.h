///
/// @file Chunk.h
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
	/// @struct ChunkVertex
	/// @brief
	///
	struct ChunkVertex
	{
		glm::vec3 position;	///< Pozièní vektor
		glm::vec3 normal;	///< Normálový vektor
	};

	///
	/// @struct ChunkIndex
	/// @brief
	///
	struct ChunkIndex
	{
		glm::uvec3 triangle1;	///< Tøi vrcholy prvního trojúhelníka segmentu
		glm::uvec3 triangle2;	///< Tøi vrcholy druhého trojúhelníka segmentu
	};


	///
	/// @class Chunk
	/// @brief
	///
	class Chunk
	{
	public:
		Chunk(vars::Vars& vars, int globalOffsetX, int globalOffsetY);
		Chunk(unsigned int width, unsigned int height, int globalOffsetX, int globalOffsetY);
		~Chunk();

		inline ChunkVertex* GetVertices() { return vertices; }
		inline void SetVertices(ChunkVertex *vertices) { this->vertices = vertices; }

		inline ChunkIndex* GetIndices() { return indices; }
		void SetIndices(ChunkIndex *indices) { this->indices = indices; }

		inline unsigned int GetWidth() { return width; }
		inline unsigned int GetHeight() { return height; }
		inline unsigned int GetIndicesSize() { return GetWidth() * GetHeight() * sizeof(ChunkIndex); }

		inline unsigned int GetVerticesWidth() { return GetWidth() + 1; }
		inline unsigned int GetVerticesHeight() { return GetHeight() + 1; }
		inline unsigned int GetVerticesSize() { return GetVerticesWidth() * GetVerticesHeight() * sizeof(ChunkVertex); }

		inline int GetGlobalOffsetX() { return globalOffsetX; }
		inline int GetGlobalOffsetY() { return globalOffsetY; }

		inline unsigned int GetDetail() { return detail; }
		inline void SetDetail(unsigned int detail) { this->detail = detail; }

	private:
		unsigned int detail = 1;	///< Detaily této èásti
		unsigned int width;		///< Šíøka této èásti mapy
		unsigned int height;	///< Výška této èásti mapy

		//	TODO: Pøedìlat => vec2 position;
		int globalOffsetX;	///< Pozièní offset chunku v rámci celé mapy na ose X
		int globalOffsetY;	///< Pozièní offset chunku v rámci celé mapy na ose Y

		ChunkVertex* vertices = nullptr;	///< Pole vertexù této èásti mapy
		ChunkIndex* indices = nullptr;		///< Pole indexù této èásti mapy
	};
}
