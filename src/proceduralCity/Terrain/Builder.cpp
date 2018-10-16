///
/// @file Builder.cpp
/// @brief
///
/// @author Daniel Dolej�ka <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/glm.hpp>
#include <Terrain/Chunk.h>
#include <Terrain/Builder.h>
#include <Terrain/HeightMap.h>


void Terrain::Builder::BuildVertices(Chunk* chunk, HeightMap* heightMap)
{
	assert(chunk != nullptr);

	//	Na�ten� pot�ebn�ch prom�nn�ch
	unsigned int width = chunk->GetVerticesWidth();
	unsigned int height = chunk->GetVerticesHeight();
	float detail = (float)chunk->GetDetail();

	Terrain::ChunkVertex* vertices;

	//	Kontrola inicializace pole s vertexy
	if (chunk->GetVertices() != nullptr)
		delete[] chunk->GetVertices();
	chunk->SetVertices(vertices = new Terrain::ChunkVertex[width * height]);

	//	Lok�ln� lambda pro jednoduch� na�ten� vertexu  
	static auto GetVertex = [&](int x, int y) -> Terrain::ChunkVertex& {
		return vertices[y * width + x];
	};

	//	Lok�ln� lambda pro v�po�et norm�ly
	static auto CalculatePosition = [&](unsigned int x, unsigned int y, glm::vec3& target)  {
		float globalX = float(float(x / detail) + chunk->GetGlobalOffsetX());
		float globalY = float(float(y / detail) + chunk->GetGlobalOffsetY());

		target.x = globalX; // ���ka
		target.y = 0; // 3D V��ka
		target.z = globalY; // 2D V��ka (Hloubka)

		target.y = heightMap->GetData(target, chunk->GetDetail()); // Sou�adnice Y je v��ka
	};

	//	Hlavn� cyklus pro vytvo�en� vertex�
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			CalculatePosition(x, y, GetVertex(x, y).position);

			/*
			printf("position[%2d, %2d] = (%f, %f, %f), chunk offset (%d, %d), detail %d;\n",
				x, y, GetVertex(x, y).position.x, GetVertex(x, y).position.y, GetVertex(x, y).position.z,
				chunk->GetGlobalOffsetX(), chunk->GetGlobalOffsetY(), chunk->GetDetail()
			);*/

			//	V�choz� norm�la
			GetVertex(x, y).normal.x = 0;
			GetVertex(x, y).normal.y = 1;
			GetVertex(x, y).normal.z = 0;
		}
	}

	//	Lok�ln� lambda pro v�po�et norm�ly
	static auto GetOrCalculatePosition = [&](unsigned int x, unsigned int y) -> glm::vec3 {
		if (x >= 0 && x < width && y >= 0 && y < height)
			return GetVertex(x, y).position;

		float globalX = float((int)x + chunk->GetGlobalOffsetX());
		float globalY = float((int)y + chunk->GetGlobalOffsetY());

		glm::vec3 result(globalX / detail, 0, globalY / detail);
		result.y = heightMap->GetData(result, chunk->GetDetail()); // Sou�adnice Y je v��ka
		return result;
	};

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

		//printf("normal[%2d, %2d] = (%f, %f, %f);\n", x, y, n.x, n.y, n.z);
		GetVertex(x, y).normal = glm::normalize(n);
	};

	//	Cyklus pro v�po�et norm�l 
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			//	Na�ten� relevantn�ch vrchol�
			CalculateNormal(x, y,
				GetOrCalculatePosition(x - 1, y),
				GetOrCalculatePosition(x, y - 1)
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
	unsigned int width = chunk->GetIndicesWidth();
	unsigned int height = chunk->GetIndicesHeight();

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

