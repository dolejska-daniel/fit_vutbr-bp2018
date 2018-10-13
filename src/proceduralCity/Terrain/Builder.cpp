///
/// @file Builder.cpp
/// @brief
///
/// @author Daniel Dolej�ka <xdolej08@stud.fit.vutbr.cz>
///
#include "Builder.h"
#include "Chunk.h"
#include "HeightMap.h"


void Terrain::Builder::BuildVertices(Chunk* chunk)
{
	assert(chunk != nullptr);

	//	Na�ten� pot�ebn�ch prom�nn�ch
	unsigned int width = chunk->GetVerticesWidth();
	unsigned int height = chunk->GetVerticesHeight();

	Terrain::HeightMap* heightMap = chunk->GetHeightMap();
	Terrain::ChunkVertex* vertices;

	//	Kontrola inicializace pole s vertexy
	if (chunk->GetVertices() != nullptr)
		delete[] chunk->GetVertices();
	chunk->SetVertices(vertices = new Terrain::ChunkVertex[width * height]);

	//	Lok�ln� lambda pro jednoduch� na�ten� vertexu
	static auto GetVertex = [&](int x, int y) -> Terrain::ChunkVertex& {
		return vertices[y * width + x];
	};

	//	Hlavn� cyklus pro vytvo�en� vertex�
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			//	Pozi�n� vektor
			GetVertex(x, y).position.x = x; // ���ka
			GetVertex(x, y).position.y = heightMap->GetData(x, y); // Sou�adnice Y je v��ka
			// GetVertex(x, y).position.y = 0;
			GetVertex(x, y).position.z = y; // 2D V��ka (Hloubka)
			printf("position[%2d, %2d] = (%f, %f, %f);\n", x, y, GetVertex(x, y).position.x, GetVertex(x, y).position.y, GetVertex(x, y).position.z);

			//	V�choz� norm�la
			GetVertex(x, y).normal.x = 0;
			GetVertex(x, y).normal.y = 1;
			GetVertex(x, y).normal.z = 0;
		}
	}

	//	Lok�ln� lambda pro v�po�et norm�ly
	static auto CalculateNormal = [&](unsigned int x, unsigned int y, glm::vec3 p1, glm::vec3 p2) {
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);
		glm::vec3 p0 = GetVertex(x, y).position;

		//	Vytvo�en� sm�rov�ch vektor� v dan� rovin�
		glm::vec3 v0 = p0 - p1;
		glm::vec3 v1 = p0 - p2;

		//	V�po�et norm�ly z cross-product-u sm�rov�ch vektor�
		glm::vec3 n = glm::cross(v1, v0);

		printf("normal[%2d, %2d] = (%f, %f, %f);\n", x, y, n.x, n.y, n.z);
		GetVertex(x, y).normal = glm::normalize(n);
	};

	//	Cyklus pro v�po�et norm�l 
	for (unsigned int y = 1; y < height; y++)
	{
		// Okrajov� troj�heln�ky po lev� hran�
		CalculateNormal(0, y,
			GetVertex(0, y - 1).position,
			GetVertex(1, y).position
		);
		for (unsigned int x = 1; x < width; x++)
		{
			//	Na�ten� relevantn�ch vrchol�
			CalculateNormal(x, y,
				GetVertex(x - 1, y).position,
				GetVertex(x, y - 1).position
			);
		}
	}
}

void Terrain::Builder::BuildIndices(Chunk* chunk)
{
	//	V�stavba index� vy�aduje ji� vystaven� vertexy
	assert(chunk != nullptr);
	assert(chunk->GetVertices() != nullptr);

	//	Na�ten� pot�ebn�ch prom�nn�ch
	unsigned int width = chunk->GetWidth();
	unsigned int height = chunk->GetHeight();

	Terrain::ChunkIndex* indices;

	//	Kontrola inicializace pole s vertexy
	if (chunk->GetIndices() != nullptr)
		delete[] chunk->GetIndices();
	chunk->SetIndices(indices = new Terrain::ChunkIndex[width * height]);

	//	Lok�ln� lambda pro jednoduch� na�ten� vertexu
	static auto GetIndex = [&](int x, int y) -> Terrain::ChunkIndex& {
		return indices[y * width + x];
	};

	//	Cyklus pro v�po�et index� 
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			int xy0 =  y      * chunk->GetVerticesWidth() + x;
			int xy1 = (y + 1) * chunk->GetVerticesWidth() + x;

			//	Prvn� troj�heln�k segmentu
			GetIndex(x, y).triangle1[0] = xy0;
			GetIndex(x, y).triangle1[1] = xy1 + 1;
			GetIndex(x, y).triangle1[2] = xy1;

			//	Druh� troj�heln�k segmentu
			GetIndex(x, y).triangle2[0] = xy0;
			GetIndex(x, y).triangle2[1] = xy0 + 1;
			GetIndex(x, y).triangle2[2] = xy1 + 1;
		}
	}
}

