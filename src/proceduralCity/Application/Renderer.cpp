///
/// @file Renderer.cpp
/// @brief Obsahuje definice funkcí třídy Application::Renderer.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <Application/Renderer.h>


using namespace Application;
using namespace ge::gl;

Renderer::Renderer(vars::Vars &vars)
	: _vars(vars)
{
}

Renderer::~Renderer()
{
}

void Renderer::Render(const std::shared_ptr<IRenderableArray> object) const
{
	object->BindVA();
	glDrawArrays(object->GetDrawMode(), 0, object->GetVB()->getSize());
}
