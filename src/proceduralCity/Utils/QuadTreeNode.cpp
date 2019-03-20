///
/// @file QuadTreeNode.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Utils/functions.h>
#include <Utils/QuadTreeNode.h>
#include <algorithm>
#include <iostream>

using namespace Utils;


QuadTreeNode::QuadTreeNode(const RectBounds& bounds)
	: IHasRectBounds(bounds)
{
	_nodes.reserve(4);
}

QuadTreeNode::~QuadTreeNode()
= default;

int QuadTreeNode::Count() const
{
	auto count = 0;
	for (const auto& node : _nodes)
		count+= node->Count();
	count+= _contents.size();
	return count;
}

std::vector<std::shared_ptr<Infrastructure::StreetSegmentQEntry>> QuadTreeNode::Contents() const
{
	return _contents;
}

std::vector<std::shared_ptr<Infrastructure::StreetSegmentQEntry>> QuadTreeNode::SubTreeContents() const
{
	auto results = Contents();
	for (const auto& node : _nodes)
		MergeVectors(results, node->SubTreeContents());

	return results;
}

std::vector<std::shared_ptr<Infrastructure::StreetSegmentQEntry>> QuadTreeNode::Query(const RectBounds& search_bounds) const
{
	// výsledky hledání
	std::vector<std::shared_ptr<Infrastructure::StreetSegmentQEntry>> results;

	// tento uzel obsahuje výsledky alespoň částečně odpovídající vyhledávané oblasti
	for (const auto& item : _contents)
	{
		if (search_bounds.IntersectsWith(item->GetRectBounds()))
		{
			results.push_back(item);
		}
	}

	for (const auto& node : _nodes)
	{
		if (node->IsEmpty())
			continue;

		// Případ 1: prohledávaná oblast je kompletně pokryta rozměry uzlu
		// (výsledky se nacházejí pouze v tomto podstromu/jeho části)
		if (node->GetRectBounds().Contains(search_bounds))
		{
			MergeVectors(results, node->Query(search_bounds));
			break;
		}

		// Případ 2: rozměry uzlu jsou kompletně pokryty prohledávanou oblastí
		// (výsledky se nacházejí v celém tomto podstromu)
		if (search_bounds.Contains(node->GetRectBounds()))
		{
			MergeVectors(results, node->SubTreeContents());
			continue;
		}

		// Případ 3: prohledávaná oblast je částěčne pokryta rozměry uzlu
		// (výsledky se nacházejí pouze v části tohoto podstromu)
		if (node->GetRectBounds().IntersectsWith(search_bounds))
		{
			MergeVectors(results, node->SubTreeContents());
		}
	}

	return results;
}

void QuadTreeNode::Insert(const std::shared_ptr<Infrastructure::StreetSegmentQEntry>& item)
{
	// v případě, že neexistují žádné poduzly, pokusíme se je vytvořit
	if (_nodes.empty())
		CreateSubNodes();

	// pro každý existující poduzel:
	// pokud daný uzel kompletně obsahuje daný objekt, vložíme jej do něj
	for (const auto& node : _nodes)
	{
		if (node->GetRectBounds().Contains(item->GetRectBounds()))
		{
			node->Insert(item);
			return;
		}
	}

	// v této situaci:
	// a) žádný z poduzlů kompletně neobsahuje daný objekt
	// b) jsme na nejnižší možné velikosti uzlu
	_contents.push_back(item);
}

void QuadTreeNode::Remove(const std::shared_ptr<Infrastructure::StreetSegmentQEntry>& item)
{
	// pro každý existující poduzel:
	// pokud daný uzel kompletně obsahuje daný objekt, pokusíme se jej z něj odstranit
	for (const auto& node : _nodes)
	{
		if (node->GetRectBounds().Contains(item->GetRectBounds()))
		{
			node->Remove(item);
			return;
		}
	}

	// v této situaci:
	// a) žádný z poduzlů kompletně neobsahuje daný objekt
	// b) jsme na nejnižší možné velikosti uzlu
	_contents.erase(std::remove(_contents.begin(), _contents.end(), item), _contents.end());
}

void QuadTreeNode::CreateSubNodes()
{
	auto bounds = GetRectBounds();
	if (bounds.Width() * bounds.Height() < 10)
		return;

	const auto half_size = bounds.size / 2.f;
	_nodes.push_back(std::make_shared<QuadTreeNode>(RectBounds(bounds.LeftBottom(), half_size)));
	_nodes.push_back(std::make_shared<QuadTreeNode>(RectBounds(bounds.LeftBottom() + glm::vec2{ half_size.x, 0.f }, half_size)));
	_nodes.push_back(std::make_shared<QuadTreeNode>(RectBounds(bounds.LeftBottom() + glm::vec2{ 0.f, half_size.y }, half_size)));
	_nodes.push_back(std::make_shared<QuadTreeNode>(RectBounds(bounds.LeftBottom() + half_size, half_size)));
}
