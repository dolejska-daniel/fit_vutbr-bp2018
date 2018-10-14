///
/// @file Chunk.h
/// @brief
///
/// @author Daniel Dolej�ka <xdolej08@stud.fit.vutbr.cz>
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
		glm::vec3 position;	///< Pozi�n� vektor
		glm::vec3 normal;	///< Norm�lov� vektor
	};

	///
	/// @struct ChunkIndex
	/// @brief
	///
	struct ChunkIndex
	{
		glm::uvec3 triangle1;	///< T�i vrcholy prvn�ho troj�heln�ka segmentu
		glm::uvec3 triangle2;	///< T�i vrcholy druh�ho troj�heln�ka segmentu
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
		unsigned int detail = 1;	///< Detaily t�to ��sti
		unsigned int width;		///< ���ka t�to ��sti mapy
		unsigned int height;	///< V��ka t�to ��sti mapy

		//	TODO: P�ed�lat => vec2 position;
		int globalOffsetX;	///< Pozi�n� offset chunku v r�mci cel� mapy na ose X
		int globalOffsetY;	///< Pozi�n� offset chunku v r�mci cel� mapy na ose Y

		ChunkVertex* vertices = nullptr;	///< Pole vertex� t�to ��sti mapy
		ChunkIndex* indices = nullptr;		///< Pole index� t�to ��sti mapy
	};
}
