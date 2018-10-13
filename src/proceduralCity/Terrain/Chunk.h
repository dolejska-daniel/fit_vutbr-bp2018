///
/// @file Chunk.h
/// @brief
///
/// @author Daniel Dolej�ka <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

#include <glm/glm.hpp>

#include "Fwd.h"


namespace Terrain
{
	struct ChunkVertex
	{
		glm::vec3 position;	///< Pozi�n� vektor
		glm::vec3 normal;	///< Norm�lov� vektor
	};

	struct ChunkIndex
	{
		glm::uvec3 triangle1;	///< T�i vrcholy prvn�ho troj�heln�ka segmentu
		glm::uvec3 triangle2;	///< T�i vrcholy druh�ho troj�heln�ka segmentu
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
		unsigned int width = 16;		///< ���ka t�to ��sti mapy
		unsigned int height = 16;	///< V��ka t�to ��sti mapy

		HeightMap* heightMap;	///< V��kov� mapa t�to ��sti

		ChunkVertex* vertices = nullptr;	///< Pole vertex� t�to ��sti mapy
		ChunkIndex* indices = nullptr;		///< Pole index� t�to ��sti mapy
	};
}
