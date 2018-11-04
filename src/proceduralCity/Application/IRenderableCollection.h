///
/// @file IRenderableCollection.h
/// @brief Obsahuje deklaraci rozhraní Application::IRenderableCollection.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <unordered_map>
#include <Application/IRenderableBase.h>


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
		virtual std::vector<std::shared_ptr<IRenderableBase>> GetRenderableObjects() = 0;
	};
}
