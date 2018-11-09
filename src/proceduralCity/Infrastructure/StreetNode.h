///
/// @file StreetNode.h
/// @brief Obsahuje deklaraci pro třídu StreetNode.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <memory>
#include <vector>
#include <map>
#include <glm/vec2.hpp>
#include <Infrastructure/Street.h>


namespace Infrastructure
{
	///
	/// @brief
	///
	class StreetNode : public std::enable_shared_from_this<StreetNode>
	{
	public:
		///
		/// @brief
		///
		enum RelativePosition
		{
			NONE = -1,	///< Unknown
			LT	= 0,	///< Left Top
			RT	= 1,	///< Right Top
			RB	= 2,	///< Right Bottom
			LB	= 3		///< Left Bottom
		};

		///
		/// @brief
		///
		StreetNode(glm::vec2 const& position, float size, std::shared_ptr<StreetNode> parent = nullptr);
		///
		/// @brief
		///
		StreetNode(glm::vec2 const& position, float size, std::map<int, std::shared_ptr<StreetNode>> const& children, std::shared_ptr<StreetNode> parent = nullptr);
		///
		/// @brief
		///
		~StreetNode();

		///
		/// @brief
		///
		bool IsRoot() const { return _isRoot; }
		///
		/// @brief
		///
		void SetRoot(bool const isRoot = true) { _isRoot = isRoot; }

		///
		/// @brief
		///
		void SetParent(std::shared_ptr<StreetNode> const& parent) { _parent = parent; }
		///
		/// @brief
		///
		std::shared_ptr<StreetNode> ReadParent() const { return _parent; }

		///
		/// @brief
		///
		bool HasChildren() const { return _hasChildren; }
		///
		/// @brief
		///
		void SetHasChildren(bool const hasChildren = true) { this->_hasChildren = hasChildren; }

		///
		/// @brief
		///
		glm::vec2 ReadPosition() const { return _position; }
		///
		/// @brief
		///
		float ReadSize() const { return _size; }

		///
		/// @brief
		///
		glm::vec2 ReadMinPosition() const { return _minPosition; }
		///
		/// @brief
		///
		glm::vec2 ReadMaxPosition() const { return _maxPosition; }

		///
		/// @brief
		///
		std::vector<StreetSegment> &GetSegments() { return _segments; }
		///
		/// @brief
		///
		std::vector<StreetSegment> ReadSegments() const { return _segments; }

		///
		/// @brief
		///
		std::map<int, std::shared_ptr<StreetNode>> &GetChildren() { return _children; }
		///
		/// @brief
		///
		std::map<int, std::shared_ptr<StreetNode>> ReadChildren() const { return _children; }

		///
		/// @brief
		///
		bool CreateChildren();
		///
		/// @brief
		///
		bool CreateParent(RelativePosition const& oldRootPosition = LT);

		///
		/// @brief
		///
		std::shared_ptr<StreetNode> GetContainingNode(StreetSegment const& segment);

		///
		/// @brief
		///
		bool Insert(StreetSegment const& segment);
		///
		/// @brief
		///
		bool Remove(StreetSegment const& segment);
		///
		/// @brief
		///
		bool Contains(StreetSegment const& segment) const;

		///
		/// @brief
		///
		bool HasPartiallyInside(StreetSegment const& segment) const;
		///
		/// @brief
		///
		bool HasInside(StreetSegment const& segment) const;
		///
		/// @brief
		///
		bool HasInside(glm::vec3 const& point) const;

		///
		/// @brief
		///
		RelativePosition RelativePositionTo(StreetSegment const& segment) const;
		///
		/// @brief
		///
		RelativePosition RelativePositionTo(glm::vec3 const& point) const;

		///
		/// @brief
		///
		RelativePosition RelativePositionFor(StreetSegment const& segment) const;
		///
		/// @brief
		///
		RelativePosition RelativePositionFor(glm::vec3 const& point) const;

	protected:
		///
		/// @brief
		///
		std::shared_ptr<StreetNode> _parent;
		///
		/// @brief
		///
		glm::vec2 _position;
		///
		/// @brief
		///
		float _size;
		///
		/// @brief
		///
		bool _isRoot;
		///
		/// @brief
		///
		bool _hasChildren;

		///
		/// @brief
		///
		glm::vec2 _minPosition{};
		///
		/// @brief
		///
		glm::vec2 _maxPosition{};

		///
		/// @brief
		///
		std::map<int, std::shared_ptr<StreetNode>> _children{};
		///
		/// @brief
		///
		std::vector<StreetSegment> _segments{};
	};

	///
	/// @brief
	///
	extern std::shared_ptr<StreetNode> StreetRootNode;
}
