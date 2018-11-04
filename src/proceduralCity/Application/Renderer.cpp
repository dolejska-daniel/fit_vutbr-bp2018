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


using namespace Application;
using namespace ge::gl;

Renderer::Renderer(vars::Vars &vars)
	: _vars(vars)
{
}

Renderer::~Renderer()
= default;


void Renderer::Render(std::shared_ptr<IRenderableArray> const& object) const
{
	object->BindVA();
	glDrawArrays(object->GetDrawMode(), 0, object->GetVB()->getSize());
}
