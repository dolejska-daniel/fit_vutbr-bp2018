///
/// @file Parcel.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec3.hpp>
#include <Application/IRenderableElementArray.h>
#include <Infrastructure/Street.h>
#include <Infrastructure/Structs/ParcelIndex.h>
#include <Infrastructure/Structs/ParcelVertex.h>


namespace Infrastructure
{
	///
	/// @brief 
	///
	class Parcel : public Application::IRenderableElementArray, public std::enable_shared_from_this<Parcel>
	{
	public:
		Parcel();
		~Parcel();

		void AddBorderPoint(glm::vec3 const& point);

		void Finish();

		bool finished;

	protected:
		std::vector<glm::vec3> _borderPoints;
	};
}
