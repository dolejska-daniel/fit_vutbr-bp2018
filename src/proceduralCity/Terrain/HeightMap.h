///
/// @file HeightMap.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <Vars/Vars.h>
#include <Utils/Curve.h>


namespace Terrain
{
	///
	/// @brief
	///
	class HeightMap
	{
	public:
		///
		/// @brief
		///
		HeightMap(vars::Vars& vars);
		///
		/// @brief
		///
		~HeightMap();

		float GenerateNoise(float globalX, float globalY) const;

		///
		/// @brief
		///
		float GetData(float x, float y, unsigned int detail) const;
		///
		/// @brief
		///
		float GetData(glm::vec2 const& v, unsigned int detail) const;
		///
		/// @brief
		///
		float GetData(glm::vec3 const& v, unsigned int detail) const;
		///
		/// @brief
		///
		float GetData(glm::vec3 const& v) const;

		///
		/// @brief
		///
		void PreprocessData(float &sample) const;

		float ilerp(float min, float max, float x) const;

	private:
		vars::Vars& _vars;	///< 

		float minNoise;	///< 
		float maxNoise; ///<

		glm::vec2* _offsets;	///< 

		std::shared_ptr<Utils::Curve2D> _curve;	///< 
	};
}
