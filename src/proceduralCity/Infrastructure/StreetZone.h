///
/// @file StreetZone.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <functional>


namespace vars
{
	class Vars;
}

namespace Infrastructure
{
	class Street;
	class StreetMap;

	///
	/// @brief 
	///
	class StreetZone
	{
	public:
		///
		/// @brief 
		///
		StreetZone(vars::Vars& vars, glm::vec2 const& center, float radius);
		///
		/// @brief 
		///
		StreetZone(vars::Vars& vars, glm::vec2 const& center, float radius,
		           std::function<void(std::shared_ptr<Street> const& street)> buildStep,
		           std::function<void(StreetMap* map, std::shared_ptr<Street> const& street)> splitStep);
		///
		/// @brief 
		///
		~StreetZone();

		///
		/// @brief 
		///
		virtual void BuildStep(std::shared_ptr<Street> const& street);
		///
		/// @brief 
		///
		virtual void SplitStep(StreetMap* map, std::shared_ptr<Street> const& street);

	protected:
		vars::Vars& _vars;

		glm::vec2 _center;
		float _radius;

		std::function<void(std::shared_ptr<Street> const& street)> _buildStep;

		std::function<void(StreetMap* map, std::shared_ptr<Street> const& street)> _splitStep;
	};
}
