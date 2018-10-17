///
/// @file IRenderableCollection.h
/// @brief Obsahuje deklaraci rozhran� Application::IRenderableCollection.
///
/// @author Daniel Dolej�ka <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <unordered_map>
#include <Application/IRenderable.h>


namespace Application
{
	///
	/// @brief 
	///
	class IRenderableCollection
	{
	public:
		///
		/// @brief 
		///
		virtual std::unordered_map<unsigned int, std::shared_ptr<IRenderable>> GetRenderableObjects() = 0;
	};
}
