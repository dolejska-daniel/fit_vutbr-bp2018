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

void Builder::BuildVertices(const std::shared_ptr<Chunk>& chunk, HeightMap* heightMap)
{
	assert(chunk != nullptr);
	assert(heightMap != nullptr);

	//	Načtení potřebných proměnných
	auto width = chunk->GetVerticesWidth();
	auto height = chunk->GetVerticesHeight();
	auto detail = chunk->GetDetail();
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
	const auto CalculatePosition = [&](const int x, const int y, glm::vec3& target)  {
		const auto globalX = scale * float(x) / float(detail) + float(globalOffsetX);
		const auto globalY = scale * float(y) / float(detail) + float(globalOffsetY);

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
			auto& vertex = GetVertex(x, y);
			CalculatePosition(x, y, vertex.position);
			/*
			printf("position[%2d, %2d] = (%f, %f, %f), chunk offset (%d, %d), _detail %d;\n",
				x, y, GetVertex(x, y).position.x, GetVertex(x, y).position.y, GetVertex(x, y).position.z,
				chunk->GetGlobalOffsetX(), chunk->GetGlobalOffsetY(), chunk->GetDetail()
			);*/

			auto pos = vertex.position.y;
			auto offsetPosDirt = pos - 42.f;
			auto offsetPosRock = pos - 24.f;
			auto grassFactor = 32.f / (abs(pos) + 0.01f);
			auto dirtFactor = offsetPosDirt * (glm::sign(offsetPosDirt) - 1.f) / (16.f * grassFactor);
			auto rockFactor = offsetPosRock * (glm::sign(offsetPosRock) + 1.f) / (16.f * grassFactor);

			auto textureMix = glm::vec3{ dirtFactor, grassFactor, rockFactor };
			vertex.textureMix = glm::normalize(textureMix);
			/*
			std::cerr << "Height: " << vertex.position.y << std::endl;
			std::cerr << "Factor: " << dirtFactor << ", " << grassFactor << ", " << rockFactor << ", " << std::endl;
			std::cerr << "Vector: " << vertex.textureMix.x << ", " << vertex.textureMix.y << ", " << vertex.textureMix.z << ", " << std::endl << std::endl;
			*/

			//	Výchozí normála
			vertex.normal.x = 0;
			vertex.normal.y = 1;
			vertex.normal.z = 0;
		}
	}

	//	Lokální lambda pro výpočet normály
	auto GetOrCalculatePosition = [&](const int x, const int y) -> glm::vec3 {
		if (x >= 0 && x < width && y >= 0 && y < height)
			return GetVertex(x, y).position;

		glm::vec3 result;
		CalculatePosition(x, y, result);
		//std::cerr << "Pos.Calc.Out: x = " << result.x << ", y = " << result.y << ", z = " << result.z << std::endl;
		return result;
	};

	//	Lokální lambda pro výpočet normály
	const auto CalculateNormal = [&](const int x, const int y) {
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);

		const auto p = GetVertex(x, y).position;

		const auto p00 = GetOrCalculatePosition(x    , y + 1);
		const auto p01 = GetOrCalculatePosition(x + 1, y    );

		const auto p10 = GetOrCalculatePosition(x + 1, y    );
		const auto p11 = GetOrCalculatePosition(x    , y - 1);

		const auto p20 = GetOrCalculatePosition(x    , y - 1);
		const auto p21 = GetOrCalculatePosition(x - 1, y    );

		const auto p30 = GetOrCalculatePosition(x - 1, y    );
		const auto p31 = GetOrCalculatePosition(x    , y + 1);

		//	Vytvoření směrových vektorů v dané rovině
		const auto v00 = p - p00;
		const auto v01 = p - p01;

		const auto v10 = p - p10;
		const auto v11 = p - p11;

		const auto v20 = p - p20;
		const auto v21 = p - p21;

		const auto v30 = p - p30;
		const auto v31 = p - p31;

		//	Výpočet normály z cross-product-u směrových vektorů
		const auto n0 = glm::cross(v00, v01);
		const auto n1 = glm::cross(v10, v11);
		const auto n2 = glm::cross(v20, v21);
		const auto n3 = glm::cross(v30, v31);

		auto n = glm::normalize(n0 + n1 + n2 + n3);
		//const auto n = n1;
		//printf("normal[%2d, %2d] = (%f, %f, %f);\n", x, y, n.x, n.y, n.z);
		GetVertex(x, y).normal = n;
	};

	//	Cyklus pro výpočet normál 
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			//	Načtení relevantních vrcholů
			CalculateNormal(x, y);
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
