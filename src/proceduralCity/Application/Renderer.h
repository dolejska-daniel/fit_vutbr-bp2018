///
/// @file Renderer.h
/// @brief Obsahuje deklaraci třídy Application::Renderer.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Vars/Vars.h>


namespace Application
{
	class IRenderableBase;
	class IRenderableArray;
	class IRenderableCollection;

	///
	/// @brief
	///
	class Renderer
	{
	public:
		///
		/// @brief
		///
		Renderer(vars::Vars &vars);
		///
		/// @brief
		///
		~Renderer();

		///
		/// @brief
		///
		void Render(std::shared_ptr<IRenderableArray> const& object) const;
		void Render(std::shared_ptr<IRenderableCollection> const& collection) const;

		void Attach(std::shared_ptr<IRenderableBase> const& object);
		void Detach(std::shared_ptr<IRenderableBase> const& object);

		void Attach(std::shared_ptr<IRenderableCollection> const& collection);
		void Detach(std::shared_ptr<IRenderableCollection> const& collection);

	private:
		///
		/// @brief
		///
		vars::Vars &_vars;
	};
}
