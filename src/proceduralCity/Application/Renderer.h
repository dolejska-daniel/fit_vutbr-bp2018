///
/// @file Renderer.h
/// @brief Obsahuje deklaraci třídy Application::Renderer.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <Vars/Vars.h>
#include <Terrain/Chunk.h>
#include "Infrastructure/Building.h"


namespace Application
{
	class IRenderableBase;
	class IRenderableArray;
	class IRenderableElementArray;
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
		Renderer();
		///
		/// @brief
		///
		~Renderer();

		///
		/// @brief
		///
		void Render(std::shared_ptr<IRenderableArray> const& object) const;
		///
		/// @brief
		///
		void Render(std::shared_ptr<IRenderableElementArray> const& object) const;

		///
		/// @brief
		///
		void Save(Terrain::Map* map, std::ofstream& output) const;
		void Save(const Infrastructure::Building& b, std::ofstream& output) const;
		void Save(const std::vector<std::shared_ptr<Infrastructure::Building>>& buildings, std::ofstream& output);
		/*
		void Render(std::shared_ptr<IRenderableCollection> const& collection) const;

		void Attach(std::shared_ptr<IRenderableBase> const& object);
		void Detach(std::shared_ptr<IRenderableBase> const& object);

		void Attach(std::shared_ptr<IRenderableCollection> const& collection);
		void Detach(std::shared_ptr<IRenderableCollection> const& collection);
		*/
	};
}
