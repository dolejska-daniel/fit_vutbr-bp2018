///
/// @file Renderer.cpp
/// @brief Obsahuje definice funkcí třídy Application::Renderer.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <Application/Renderer.h>
#include <Application/IRenderableArray.h>
#include <Application/IRenderableElementArray.h>
#include <Terrain/Chunk.h>
#include <Terrain/Map.h>
#include <fstream>


using namespace ge::gl;
using namespace Application;

Renderer::Renderer(vars::Vars& vars)
	: _vars(vars)
{
}

Renderer::~Renderer()
= default;


void Renderer::Render(std::shared_ptr<IRenderableArray> const& object) const
{
	object->BindVA();
	glDrawArrays(object->GetDrawMode(), 0, static_cast<GLsizei>(object->GetRenderableCount()));
}

void Renderer::Render(std::shared_ptr<IRenderableElementArray> const& object) const
{
	object->BindVA();
	//object->BindVB();
	glDrawElements(object->GetDrawMode(), static_cast<GLsizei>(object->GetVA()->getElement()->getSize()), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Save(Terrain::Map *map, std::ofstream& output) const
{
	if (!output.is_open())
		return;

	size_t offset = 0;
	const auto chunks = map->GetChunks();
	output << "#\n# Terrain chunks\n#" << std::endl;
	for (const auto& ch : chunks)
	{
		output << "\n# Chunk " << ch.first << " vertices" << std::endl;
		output << "g chunk_" << ch.first << std::endl;
		const auto chunk = ch.second;
		const auto vertices = chunk->GetVertices();
		for (size_t i = 0; i < chunk->GetVerticesWidth() * chunk->GetVerticesHeight(); ++i)
		{
			const auto v = vertices[i];
			output
				<< "v"
				<< " " << v.position.x
				<< " " << v.position.y
				<< " " << v.position.z << std::endl;
		}

		output << "# Chunk " << ch.first << " normals" << std::endl;
		for (size_t i = 0; i < chunk->GetVerticesWidth() * chunk->GetVerticesHeight(); ++i)
		{
			const auto v = vertices[i];
			output
				<< "vn"
				<< " " << v.normal.x
				<< " " << v.normal.y
				<< " " << v.normal.z << std::endl;
		}

		auto new_offset = 0;
		const auto indices = chunk->GetIndices();
		output << "# Chunk " << ch.first << " indices" << std::endl;
		for (size_t i = 0; i < chunk->GetIndicesWidth() * chunk->GetIndicesHeight(); ++i)
		{
			const auto idx = indices[i];
			output
				<< "f"
				<< " " << idx.triangle1.x + 1 + offset
				<< " " << idx.triangle1.y + 1 + offset
				<< " " << idx.triangle1.z + 1 + offset << std::endl;

			output
				<< "f"
				<< " " << idx.triangle2.x + 1 + offset
				<< " " << idx.triangle2.y + 1 + offset
				<< " " << idx.triangle2.z + 1 + offset << std::endl;

			new_offset = idx.triangle2.z + 1 + offset;
		}
		offset = new_offset;

		output << "end" << std::endl;
	}
}
