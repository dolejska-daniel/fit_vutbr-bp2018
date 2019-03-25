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
	enum ParcelType
	{
		STREET,
		BUILDING,
	};

	///
	/// @brief 
	///
	class Parcel : public Application::IRenderableElementArray, public std::enable_shared_from_this<Parcel>
	{
	public:
		Parcel();
		explicit Parcel(ParcelType type);
		~Parcel();

		bool AddBorderPoint(glm::vec3 point);
		std::vector<glm::vec3> const& GetBorderPoints() const;

		glm::vec3 GetMidpoint();

		void Shrink(float size);
		void Expand(float size);

		void Finish();

		bool finished = false;
		std::vector<glm::vec3> borderPoints;
		ParcelType type;
	};
}
