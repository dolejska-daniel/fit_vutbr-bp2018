///
/// @file Builder.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <glm/glm.hpp>
#include <Terrain/Chunk.h>
#include <Terrain/Builder.h>
#include <Terrain/HeightMap.h>


using namespace Terrain;

void Builder::BuildVertices(std::shared_ptr<Chunk> chunk, HeightMap* heightMap)
{
	assert(chunk != nullptr);

	//	Načtení potřebných proměnných
	auto width = chunk->GetVerticesWidth();
	auto height = chunk->GetVerticesHeight();
	auto detail = static_cast<float>(chunk->GetDetail());
	auto scale = chunk->GetScale();
	auto globalOffsetX = chunk->GetGlobalOffsetX();
	auto globalOffsetY = chunk->GetGlobalOffsetY();

	//	Kontrola inicializace pole s vertexy
	if (chunk->GetVertices() != nullptr)
		delete[] chunk->GetVertices();
	auto vertices = new ChunkVertex[width * height];

	//	Lokální lambda pro jednoduché načtení vertexu  
	auto GetVertex = [&](const int x, const int y) -> ChunkVertex& {
		return vertices[y * width + x];
	};

	//	Lokální lambda pro výpočet normály
	auto CalculatePosition = [&](const unsigned int x, const unsigned int y, glm::vec3& target)  {
		const auto globalX = float(float(scale * x / detail) + globalOffsetX);
		const auto globalY = float(float(scale * y / detail) + globalOffsetY);

		target.x = globalX; // Šířka
		target.y = 0; // 3D Výška
		target.z = globalY; // 2D Výška (Hloubka)

		target.y = heightMap->GetData(target, detail); // Souřadnice Y je výška
	};

	//	Hlavní cyklus pro vytvoření vertexů
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			CalculatePosition(x, y, GetVertex(x, y).position);
			/*
			printf("position[%2d, %2d] = (%f, %f, %f), chunk offset (%d, %d), _detail %d;\n",
				x, y, GetVertex(x, y).position.x, GetVertex(x, y).position.y, GetVertex(x, y).position.z,
				chunk->GetGlobalOffsetX(), chunk->GetGlobalOffsetY(), chunk->GetDetail()
			);*/

			//	Výchozí normála
			GetVertex(x, y).normal.x = 0;
			GetVertex(x, y).normal.y = 1;
			GetVertex(x, y).normal.z = 0;
		}
	}

	//	Lokální lambda pro výpočet normály
	auto GetOrCalculatePosition = [&](const unsigned int x, const unsigned int y) -> glm::vec3 {
		if (x >= 0 && x < width && y >= 0 && y < height)
			return GetVertex(x, y).position;

		const auto globalX = float(float(static_cast<int>(x) / detail) + chunk->GetGlobalOffsetX());
		const auto globalY = float(float(static_cast<int>(y) / detail) + chunk->GetGlobalOffsetY());

		glm::vec3 result(globalX / detail, 0, globalY / detail);
		result.y = heightMap->GetData(result, chunk->GetDetail()); // Souřadnice Y je výška
		return result;
	};

	//	Lokální lambda pro výpočet normály
	auto CalculateNormal = [&](const unsigned int x, const unsigned int y, glm::vec3 const& p1, glm::vec3 const& p2) {
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);

		const auto p0 = GetVertex(x, y).position;

		//	Vytvoření směrových vektorů v dané rovině
		const auto v0 = p0 - p1;
		const auto v1 = p0 - p2;

		//	Výpočet normály z cross-product-u směrových vektorů
		const auto n = glm::cross(v1, v0);

		//printf("normal[%2d, %2d] = (%f, %f, %f);\n", x, y, n.x, n.y, n.z);
		GetVertex(x, y).normal = glm::normalize(n);
	};

	//	Cyklus pro výpočet normál 
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			//	Načtení relevantních vrcholů
			CalculateNormal(x, y,
				GetOrCalculatePosition(x - 1, y),
				GetOrCalculatePosition(x, y - 1)
			);
		}
	}

	//	Cyklus pro scale vertexů
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			//GetVertex(x, y).position *= chunk->GetScale();
		}
	}

	chunk->SetVertices(vertices);
}

void Builder::BuildIndices(std::shared_ptr<Chunk> const& chunk)
{
	//	Výstavba indexů vyžaduje již vystavené vertexy
	assert(chunk != nullptr);
	assert(chunk->GetVertices() != nullptr);

	//	Načtení potřebných proměnných
	const auto width = chunk->GetIndicesWidth();
	const auto height = chunk->GetIndicesHeight();

	//	Kontrola inicializace pole s vertexy
	if (chunk->GetIndices() != nullptr)
		delete[] chunk->GetIndices();
	auto indices = new ChunkIndex[width * height];

	//	Lokální lambda pro jednoduché načtení vertexu
	auto GetIndex = [&](const int x, const int y) -> ChunkIndex& {
		return indices[y * width + x];
	};

	//	Cyklus pro výpočet indexů 
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			const int xy0 =  y      * chunk->GetVerticesWidth() + x;
			const int xy1 = (y + 1) * chunk->GetVerticesWidth() + x;

			//	První trojúhelník segmentu
			GetIndex(x, y).triangle1[0] = xy0;
			GetIndex(x, y).triangle1[1] = xy1 + 1;
			GetIndex(x, y).triangle1[2] = xy1;

			//	Druhý trojúhelník segmentu
			GetIndex(x, y).triangle2[0] = xy0;
			GetIndex(x, y).triangle2[1] = xy0 + 1;
			GetIndex(x, y).triangle2[2] = xy1 + 1;
		}
	}

	chunk->SetIndices(indices);
}
