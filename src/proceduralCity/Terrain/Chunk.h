///
/// @file Chunk.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec3.hpp>
#include <Vars/Vars.h>
#include <Terrain/Fwd.h>


namespace Terrain
{
	///
	/// @struct ChunkVertex
	/// @brief
	///
	struct ChunkVertex
	{
		glm::vec3 position;	///< Poziční vektor
		glm::vec3 normal;	///< Normálový vektor
	};

	///
	/// @struct ChunkIndex
	/// @brief
	///
	struct ChunkIndex
	{
		glm::uvec3 triangle1;	///< Tři vrcholy prvního trojúhelníka segmentu
		glm::uvec3 triangle2;	///< Tři vrcholy druhého trojúhelníka segmentu
	};


	///
	/// @class Chunk
	/// @brief
	///
	class Chunk
	{
	public:
		Chunk(vars::Vars& vars, int globalOffsetX, int globalOffsetY);
		~Chunk();

		inline ChunkVertex* GetVertices() { return vertices; }
		inline void SetVertices(ChunkVertex *vertices) { this->vertices = vertices; }

		inline ChunkIndex* GetIndices() { return indices; }
		void SetIndices(ChunkIndex *indices) { this->indices = indices; }

		inline unsigned int GetWidth() { return width; }
		inline unsigned int GetHeight() { return height; }

		inline unsigned int GetIndicesWidth() { return GetWidth() * GetDetail(); }
		inline unsigned int GetIndicesHeight() { return GetHeight() * GetDetail(); }
		inline unsigned int GetIndicesSize() { return GetIndicesWidth() * GetIndicesHeight() * sizeof(ChunkIndex); }

		inline unsigned int GetVerticesWidth() { return (GetWidth() + 1) * GetDetail(); }
		inline unsigned int GetVerticesHeight() { return (GetHeight() + 1) * GetDetail(); }
		inline unsigned int GetVerticesSize() { return GetVerticesWidth() * GetVerticesHeight() * sizeof(ChunkVertex); }

		inline int GetGlobalOffsetX() { return globalOffsetX; }
		inline int GetGlobalOffsetY() { return globalOffsetY; }

		inline unsigned int GetDetail() { return detail; }
		inline void SetDetail(unsigned int detail) { this->detail = detail; }

	private:
		unsigned int detail;	///< Detaily této části
		unsigned int width;		///< Šířka této části mapy
		unsigned int height;	///< Výška této části mapy

		//	TODO: Předělat => vec2 position;
		int globalOffsetX;	///< Poziční offset chunku v rámci celé mapy na ose X
		int globalOffsetY;	///< Poziční offset chunku v rámci celé mapy na ose Y

		ChunkVertex* vertices = nullptr;	///< Pole vertexů této části mapy
		ChunkIndex* indices = nullptr;		///< Pole indexů této části mapy
	};
}
