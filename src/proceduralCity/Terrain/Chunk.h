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
#include <Terrain/Structs/ChunkVertex.h>
#include <Terrain/Structs/ChunkIndex.h>
#include <Application/IRenderableElementArray.h>


namespace Terrain
{
	///
	/// @brief
	///
	class Chunk : public Application::IRenderableElementArray
	{
	public:
		///
		/// @brief
		///
		Chunk(vars::Vars& vars, int globalOffsetX, int globalOffsetY);
		///
		/// @brief
		///
		~Chunk();

		///
		/// @brief
		///
		ChunkVertex* GetVertices() const { return _vertices; }
		///
		/// @brief
		///
		void SetVertices(ChunkVertex* vertices);

		///
		/// @brief
		///
		ChunkIndex* GetIndices() const { return _indices; }
		///
		/// @brief
		///
		void SetIndices(ChunkIndex* indices);

		///
		/// @brief
		///
		unsigned int GetWidth() const { return _width; }
		///
		/// @brief
		///
		unsigned int GetHeight() const { return _height; }
		///
		/// @brief
		///
		float GetScale() const { return _scale; }

		///
		/// @brief
		///
		unsigned int GetIndicesWidth() const;

		///
		/// @brief
		///
		unsigned int GetIndicesHeight() const;

		///
		/// @brief
		///
		unsigned int GetIndicesSize() const;

		///
		/// @brief
		///
		unsigned int GetVerticesWidth() const;
		///
		/// @brief
		///
		unsigned int GetVerticesHeight() const;
		///
		/// @brief
		///
		unsigned int GetVerticesSize() const;

		///
		/// @brief
		///
		int GetGlobalOffsetX() const { return _globalOffsetX; }
		///
		/// @brief
		///
		int GetGlobalOffsetY() const { return _globalOffsetY; }

		///
		/// @brief
		///
		unsigned int GetDetail() const { return _detail; }
		///
		/// @brief
		///
		void SetDetail(const unsigned int detail) { this->_detail = detail; }

	private:
		unsigned int _detail;	///< Detaily této části
		unsigned int _width;	///< Šířka této části mapy
		unsigned int _height;	///< Výška této části mapy
		float _scale;			///< Násobič velikosti této části mapy

		//	TODO: Předělat => vec2 position;
		int _globalOffsetX;	///< Poziční offset chunku v rámci celé mapy na ose X
		int _globalOffsetY;	///< Poziční offset chunku v rámci celé mapy na ose Y

		ChunkVertex* _vertices = nullptr;	///< Pole vertexů této části mapy
		ChunkIndex* _indices = nullptr;		///< Pole indexů této části mapy
	};
}
