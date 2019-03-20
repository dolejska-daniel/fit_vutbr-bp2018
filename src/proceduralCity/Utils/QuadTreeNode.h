///
/// @file QuadTreeNode.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
/// Implementace založena na https://www.codeproject.com/Articles/30535/A-Simple-QuadTree-Implementation-in-C
///
#pragma once
#include <vector>
#include <memory>
#include <type_traits>
#include <Utils/IHasRectBounds.h>
#include <Infrastructure/Structs/StreetSegment.h>


namespace Utils
{
	///
	/// @brief
	///
	class QuadTreeNode : public IHasRectBounds
	{
	public:
		///
		/// @brief
		///
		QuadTreeNode(const RectBounds& bounds);
		///
		/// @brief
		///
		~QuadTreeNode();

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
		std::vector<std::shared_ptr<Infrastructure::StreetSegmentQEntry>> Contents() const;
		///
		/// @brief
		///
		std::vector<std::shared_ptr<Infrastructure::StreetSegmentQEntry>> SubTreeContents() const;

		///
		/// @brief
		///
		std::vector<std::shared_ptr<Infrastructure::StreetSegmentQEntry>> Query(const RectBounds& search_bounds) const;
		///
		/// @brief
		///
		void Insert(const std::shared_ptr<Infrastructure::StreetSegmentQEntry>& item);
		///
		/// @brief
		///
		void Remove(const std::shared_ptr<Infrastructure::StreetSegmentQEntry>& item);

	protected:
		///
		/// @brief
		///
		void CreateSubNodes();

		///
		/// @brief
		///
		std::vector<std::shared_ptr<Infrastructure::StreetSegmentQEntry>> _contents;
		///
		/// @brief
		///
		std::vector<std::shared_ptr<QuadTreeNode>> _nodes;
	};
}
