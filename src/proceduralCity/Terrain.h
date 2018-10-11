///
/// @file Terrain.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once

#include <geGL/geGL.h>
#include <glm/vec3.hpp>

enum TerrainVertexIndex
{
	Position = 0,
	Normal = 1,
};

class Terrain
{

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
	};

public:
	Terrain(int width, int height);
	~Terrain();

	glm::vec3 *BuildVertices();
	inline unsigned int GetVerticesCount() { return vWidth * vHeight * GetVertexOffsetY(1); };
	inline size_t GetVerticesSize() { return GetVerticesCount() * sizeof(glm::vec3); };

	inline unsigned int GetVertexOffsetX(unsigned int x) { return x * vWidth * GetVertexOffsetY(1); };
	inline unsigned int GetVertexOffsetY(unsigned int y) { return y * 2; };
	inline unsigned int GetVertexOffset(unsigned int x, unsigned int y, TerrainVertexIndex type) {
		return GetVertexOffsetX(x) + GetVertexOffsetY(y) + (unsigned int)type;
	};

	glm::uvec3	*BuildIndices();
	inline unsigned int GetIndicesCount() { return iWidth * iHeight * GetIndexOffsetY(1); };
	inline size_t GetIndicesSize() { return GetIndicesCount() * sizeof(glm::uvec3); };

	inline unsigned int GetIndexOffsetX(unsigned int x) { return x * iWidth * GetVertexOffsetY(1); };
	inline unsigned int GetIndexOffsetY(unsigned int y) { return y * 2; };
	inline unsigned int GetIndexOffset(unsigned int x, unsigned int y) { return GetIndexOffsetX(x) + GetIndexOffsetY(y); };

	void Render();

private:
	int width, height;

	int vWidth, vHeight;
	glm::vec3	*vertices;	///< Vertex data

	int iWidth, iHeight;
	glm::uvec3	*indices;	///< Index data
	
};

