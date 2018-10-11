///
/// @file Terrain.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <stdio.h>
#include <stdlib.h>

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/noise.hpp>

#include "Terrain.h"

using namespace ge::gl;


Terrain::Terrain(int width, int height)
	: width(width), vWidth(width + 1), iWidth(width)
	, height(height), vHeight(height + 1), iHeight(height)
{
	//	Alokace pole pro vertexy
	vertices = new glm::vec3[GetVerticesCount()];
	//vertices = (glm::vec3 *) malloc(GetVerticesSize());

	//	Alokace pole pro indexy
	indices = new glm::uvec3[GetIndicesCount()];
}

Terrain::~Terrain()
{
	//	Free vertex data
	delete[] vertices;

	//	Free index data
	delete[] indices;
}


glm::vec3 *Terrain::BuildVertices()
{
    int persistence = 2 ^ 6;

	//	Inicializace pole
	for (int x = 0; x < vHeight; x++)
	{
		for (int y = 0; y < vWidth; y++)
		{
			//	Poziční vektor
			vertices[GetVertexOffset(x, y, TerrainVertexIndex::Position)].x = x;
			vertices[GetVertexOffset(x, y, TerrainVertexIndex::Position)].y = y; // Souřadnice Y je výška
			vertices[GetVertexOffset(x, y, TerrainVertexIndex::Position)].z = y;
			vertices[GetVertexOffset(x, y, TerrainVertexIndex::Position)].y = (float)glm::perlin(glm::vec2(x / (float)8.f, y / (float)8.f)) * persistence;

			vertices[GetVertexOffset(x, y, TerrainVertexIndex::Normal)].x = 0;
			vertices[GetVertexOffset(x, y, TerrainVertexIndex::Normal)].y = 1;
			vertices[GetVertexOffset(x, y, TerrainVertexIndex::Normal)].z = 0;
		}
	}

	//	Sestavení normál
	for (int x = 1; x < vHeight - 1; x++)
	{
		for (int y = 1; y < vWidth - 1; y++)
		{
			glm::vec3 offset(1.0, 1.0, 0);

			float hL = vertices[GetVertexOffset(x - 1, y, TerrainVertexIndex::Position)].y;
			float hR = vertices[GetVertexOffset(x + 1, y, TerrainVertexIndex::Position)].y;
			float hD = vertices[GetVertexOffset(y, y - 1, TerrainVertexIndex::Position)].y;
			float hU = vertices[GetVertexOffset(x, y + 1, TerrainVertexIndex::Position)].y;

			//	Normála
			glm::vec3 n(0);
			n.x = hL - hR;
			n.y = hD - hU;
			n.z = 2;
			vertices[GetVertexOffset(x, y, TerrainVertexIndex::Normal)] = glm::normalize(n);
		}
	}

	return vertices;
}


glm::uvec3 *Terrain::BuildIndices()
{
	//	Inicializace pole
	for (int x = 0; x < iHeight; x++)
	{
		for (int y = 0; y < iWidth; y++)
		{
			/*
			for (int i = 0; i < 2; i++)
			{
				indices[x][y][i][0] = 0;
				indices[x][y][i][1] = 0;
				indices[x][y][i][2] = 0;
			}
			*/
			int xy0 =  x      * vHeight + y;
			int xy1 = (x + 1) * vHeight + y;

			//	Triangle 1
			indices[GetIndexOffset(x, y)][0] = xy0;
			indices[GetIndexOffset(x, y)][1] = xy1 + 1;
			indices[GetIndexOffset(x, y)][2] = xy1;

			//	Triangle 2
			indices[GetIndexOffset(x, y) + 1][0] = xy0;
			indices[GetIndexOffset(x, y) + 1][1] = xy0 + 1;
			indices[GetIndexOffset(x, y) + 1][2] = xy1 + 1;
		}
	}

	return indices;
}

void Terrain::Render()
{
	/*
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	*/

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glDrawElements(GL_TRIANGLES, 3 * sizeof(unsigned int), GL_UNSIGNED_INT, (const void *)0);
}
