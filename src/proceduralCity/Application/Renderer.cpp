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
