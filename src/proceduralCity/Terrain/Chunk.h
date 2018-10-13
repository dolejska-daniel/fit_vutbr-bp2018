///
/// @file Chunk.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

#include <glm/glm.hpp>

#include "Fwd.h"


namespace Terrain
{
	struct ChunkVertex
	{
		glm::vec3 position;	///< Pozièní vektor
		glm::vec3 normal;	///< Normálový vektor
	};

	struct ChunkIndex
	{
		glm::uvec3 triangle1;	///< Tøi vrcholy prvního trojúhelníka segmentu
		glm::uvec3 triangle2;	///< Tøi vrcholy druhého trojúhelníka segmentu
	};


	class Chunk
	{
		friend class HeightMap;

	public:
		Chunk(Map* map);
		~Chunk();

		inline ChunkVertex* GetVertices() { return vertices; }
		inline void SetVertices(ChunkVertex *vertices) { this->vertices = vertices; }

		inline ChunkIndex* GetIndices() { return indices; }
		void SetIndices(ChunkIndex *indices) { this->indices = indices; }

		inline HeightMap* GetHeightMap() { return heightMap; }
		inline unsigned int GetWidth() { return width; }
		inline unsigned int GetHeight() { return height; }
		inline unsigned int GetIndicesSize() { return GetWidth() * GetHeight() * sizeof(ChunkIndex); }

		inline unsigned int GetVerticesWidth() { return GetWidth() + 1; }
		inline unsigned int GetVerticesHeight() { return GetHeight() + 1; }
		inline unsigned int GetVerticesSize() { return GetVerticesWidth() * GetVerticesHeight() * sizeof(ChunkVertex); }

	private:
		unsigned int width = 16;		///< Šíøka této èásti mapy
		unsigned int height = 16;	///< Výška této èásti mapy

		HeightMap* heightMap;	///< Výšková mapa této èásti

		ChunkVertex* vertices = nullptr;	///< Pole vertexù této èásti mapy
		ChunkIndex* indices = nullptr;		///< Pole indexù této èásti mapy
	};
}
