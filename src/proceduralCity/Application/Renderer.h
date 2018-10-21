///
/// @file Renderer.h
/// @brief Obsahuje deklaraci třídy Application::Renderer.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Vars/Vars.h>
#include <geGL/VertexArray.h>
#include <Application/IRenderableBase.h>
#include <Application/IRenderableArray.h>
#include <Application/IRenderableCollection.h>


namespace Application
{
	class Renderer
	{
	public:
		Renderer(vars::Vars &vars);
		~Renderer();

		void Render(const std::shared_ptr<IRenderableArray> object) const;
		void Render(const std::shared_ptr<IRenderableCollection> collection) const;

		void Attach(std::shared_ptr<IRenderableBase> object);
		void Detach(std::shared_ptr<IRenderableBase> object);

		void Attach(std::shared_ptr<IRenderableCollection> collection);
		void Detach(std::shared_ptr<IRenderableCollection> collection);

	private:
		vars::Vars &_vars;
	};
}
