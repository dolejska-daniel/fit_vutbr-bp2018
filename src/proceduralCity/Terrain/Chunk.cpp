///
/// @file Chunk.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Vars/Vars.h>
#include <Terrain/Map.h>
#include <Terrain/Chunk.h>
#include <Application/Application.h>


using namespace Terrain;

Chunk::Chunk(const int globalOffsetX, const int globalOffsetY)
	: _globalOffsetX(globalOffsetX), _globalOffsetY(globalOffsetY)
{
	_detail = Application::Vars.getUint32("terrain.detail");
	_width  = Application::Vars.getUint32("terrain.chunk.width");
	_height = Application::Vars.getUint32("terrain.chunk.height");
	_scale  = Application::Vars.getFloat("terrain.chunk.scale");

	SetDrawMode(GL_TRIANGLES);
	auto va = CreateVA();
	BindVA();

	auto vb = CreateVB();
	BindVB();
	vb->alloc(GetVerticesSize());
	va->addAttrib(vb, 0, 3, GL_FLOAT, 3 * sizeof(glm::vec3), 0 * sizeof(glm::vec3));
	va->addAttrib(vb, 1, 3, GL_FLOAT, 3 * sizeof(glm::vec3), 1 * sizeof(glm::vec3));
	va->addAttrib(vb, 2, 3, GL_FLOAT, 3 * sizeof(glm::vec3), 2 * sizeof(glm::vec3));

	auto ib = CreateIB();
	BindIB();
	ib->alloc(GetIndicesSize());
	va->addElementBuffer(ib);
}

Chunk::~Chunk()
{
	delete[] _vertices;
	delete[] _indices;
}

void Chunk::SetVertices(ChunkVertex* vertices)
{
	_vertices = vertices;
	if (GetVerticesSize() > GetVB()->getSize())
		GetVB()->realloc(GetVerticesSize(), ge::gl::Buffer::ReallocFlags::KEEP_ID);
	GetVB()->setData(vertices, GetVerticesSize());
}

void Chunk::SetIndices(ChunkIndex* indices)
{
	_indices = indices;
	if (GetIndicesSize() > GetIB()->getSize())
		GetIB()->realloc(GetIndicesSize(), ge::gl::Buffer::ReallocFlags::KEEP_ID);
	GetIB()->setData(indices, GetIndicesSize());
}

unsigned Chunk::GetIndicesWidth() const
{
	return GetWidth() * GetDetail();
}

unsigned Chunk::GetIndicesHeight() const
{
	return GetHeight() * GetDetail();
}

unsigned Chunk::GetIndicesSize() const
{
	return GetIndicesWidth() * GetIndicesHeight() * sizeof(ChunkIndex);
}

unsigned Chunk::GetVerticesWidth() const
{
	//	Počet vrcholů je vždy +1 (délka 2 = vrchol 0, 1, 2 => 3)
	return (GetWidth() + 1) * GetDetail();
}

unsigned Chunk::GetVerticesHeight() const
{
	//	Počet vrcholů je vždy +1 (délka 2 = vrchol 0, 1, 2 => 3)
	return (GetHeight() + 1) * GetDetail();
}

unsigned Chunk::GetVerticesSize() const
{
	return GetVerticesWidth() * GetVerticesHeight() * sizeof(ChunkVertex);
}
