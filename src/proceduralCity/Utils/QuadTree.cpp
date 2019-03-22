///
/// @file QuadTree.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Utils/QuadTree.h>
#include <Infrastructure/Structs/StreetSegment.h>

using namespace Utils;


std::shared_ptr<QuadTree> Utils::StreetQuadTree = nullptr;

QuadTree::QuadTree(const RectBounds& bounds)
	: IHasRectBounds(bounds), _root(std::make_shared<QuadTreeNode>(bounds))
{
}

QuadTree::~QuadTree()
= default;

int QuadTree::Count() const
{
	return _root->Count();
}

std::vector<Infrastructure::StreetSegment> QuadTree::Query(const RectBounds& search_bounds) const
{
	std::vector<std::shared_ptr<Infrastructure::StreetSegmentQEntry>> query_results;
	_root->Query(search_bounds, query_results);

	std::vector<Infrastructure::StreetSegment> results;
	results.reserve(query_results.size());
	for (const auto& query_result : query_results)
		results.push_back(query_result->segment);

	return results;
}

void QuadTree::Insert(const Infrastructure::StreetSegment& segment) const
{
	_root->Insert(std::make_shared<Infrastructure::StreetSegmentQEntry>(segment));
}

void QuadTree::Remove(const Infrastructure::StreetSegment& segment) const
{
	_root->Remove(std::make_shared<Infrastructure::StreetSegmentQEntry>(segment));
}
