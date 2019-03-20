///
/// @file QuadTree.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
/// Implementace založena na https://www.codeproject.com/Articles/30535/A-Simple-QuadTree-Implementation-in-C
///
#pragma once
#include <vector>
#include <memory>
#include <Utils/IHasRectBounds.h>
#include <Utils/QuadTreeNode.h>
#include <Infrastructure/Structs/StreetSegment.h>


namespace Utils
{
	///
	/// @brief
	///
	class QuadTree : public IHasRectBounds
	{
	public:
		///
		/// @brief
		///
		QuadTree(const RectBounds& bounds);
		///
		/// @brief
		///
		~QuadTree();

		///
		/// @brief
		///
		bool IsEmpty() const { return GetRectBounds().IsEmpty() || Count() == 0; }
		///
		/// @brief
		///
		int Count() const;

		///
		/// @brief
		///
		std::vector<Infrastructure::StreetSegment> Query(const RectBounds& search_bounds) const;
		///
		/// @brief
		///
		void Insert(const Infrastructure::StreetSegment& segment) const;
		///
		/// @brief
		///
		void Remove(const Infrastructure::StreetSegment& segment) const;

	protected:
		///
		/// @brief
		///
		std::shared_ptr<QuadTreeNode> _root;
	};

	///
	/// @brief
	///
	extern std::shared_ptr<QuadTree> StreetQuadTree;
}
