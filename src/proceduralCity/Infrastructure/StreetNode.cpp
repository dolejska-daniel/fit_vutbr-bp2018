///
/// @file StreetNode.cpp
/// @brief Obsahuje definice funkcí pro třídu StreetNode.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Infrastructure/StreetNode.h>
#include <Infrastructure/Street.h>
#include <glm/integer.hpp>


std::shared_ptr<Infrastructure::StreetNode> Infrastructure::StreetRootNode = nullptr;


using namespace Infrastructure;

StreetNode::StreetNode(glm::vec2 const& position, const float size)
	: _position(position), _size(size), _isRoot(false), _hasChildren(false)
{
	_minPosition = position - this->_size;
	_maxPosition = position + this->_size;
}

StreetNode::StreetNode(glm::vec2 const& position, const float size, std::map<int, std::shared_ptr<StreetNode>> const& children)
	: _position(position), _size(size), _isRoot(false), _hasChildren(false)
{
	_minPosition = position - this->_size;
	_maxPosition = position + this->_size;

	for (auto &node : children)
	{
		_children[node.first] = node.second;
		if (node.second != nullptr)
			SetHasChildren(true);
	}

	if (HasChildren())
		CreateChildren();
}

StreetNode::~StreetNode()
= default;


void StreetNode::CreateChildren()
{
	glm::vec2 childPosition;
	const auto childSize = _size / 2.f;

	// Left Top
	childPosition.x = _position.x - childSize;
	childPosition.y = _position.y + childSize;
	if (_children[LT] == nullptr)
		_children[LT] = std::make_shared<StreetNode>(childPosition, childSize);

	// Right Top
	childPosition.x = _position.x + childSize;
	if (_children[RT] == nullptr)
		_children[RT] = std::make_shared<StreetNode>(childPosition, childSize);

	// Right Bottom
	childPosition.y = _position.y - childSize;
	if (_children[RB] == nullptr)
		_children[RB] = std::make_shared<StreetNode>(childPosition, childSize);

	// Left Bottom
	childPosition.x = _position.x - childSize;
	if (_children[LB] == nullptr)
		_children[LB] = std::make_shared<StreetNode>(childPosition, childSize);

	SetHasChildren(true);
}

bool StreetNode::CreateParent(RelativePosition const& oldRootPosition)
{
	if (StreetRootNode == nullptr)
		return false;

	if (!IsRoot())
		throw std::exception("Only root node can create parent.");

	auto position = _position;
	const auto size = _size * 2.f;
	std::map<int, std::shared_ptr<StreetNode>> children{
		{oldRootPosition, shared_from_this()},
	};

	switch (oldRootPosition)
	{
	case LT:
		position.x += _size;
		position.y -= _size;
		break;
	case RT:
		position.x -= _size;
		position.y -= _size;
		break;
	case RB:
		position.x -= _size;
		position.y += _size;
		break;
	case LB:
		position.x += _size;
		position.y += _size;
		break;
	}

	this->SetRoot(false);
	StreetRootNode = std::make_shared<StreetNode>(position, size, children);
	StreetRootNode->SetRoot();
	return true;
}

std::shared_ptr<StreetNode> StreetNode::GetContainingNode(StreetSegment const& segment)
{
	if (Contains(segment))
		return shared_from_this();

	if (HasChildren())
	{
		for (auto &node : _children)
		{
			auto result = node.second->GetContainingNode(segment);
			if (result) return result;
		}
	}

	return nullptr;
}

bool StreetNode::Insert(StreetSegment const& segment)
{
	if (IsInside(segment))
	{
		//	Koncový i počáteční bod sem patří
		if (!HasChildren())
			CreateChildren();

		//	Nejdříve se pokusíme nalézt přesnější match
		for (auto &node : _children)
			if (node.second->Insert(segment))
				return true;

		//	Vložení segmentu
		_segments.push_back(segment);
		return true;
	}
	
	if (IsRoot())
	{
		//	Vytvoření nadřazeného uzlu
		if (CreateParent(RelativePositionTo(segment)))
			//	Pokus o vložení do nadžazeného uzlu
			return StreetRootNode->Insert(segment);
	}

	return false;
}

bool StreetNode::Remove(StreetSegment const& segment)
{
	//	TODO: Specifikovat přesněji?

	if (!_segments.empty())
	{
		const auto r = std::find(_segments.begin(), _segments.end(), segment);
		if (r != _segments.end())
		{
			_segments.erase(r);
			return true;
		}
	}

	if (HasChildren())
	{
		for (auto &node : _children)
			if (node.second != nullptr
				&& node.second->Remove(segment))
				return true;
	}

	return false;
}

bool StreetNode::Contains(StreetSegment const& segment) const
{
	const auto r = std::find(_segments.begin(), _segments.end(), segment);
	return r != _segments.end();
}

bool StreetNode::IsPartiallyInside(StreetSegment const& segment) const
{
	return IsInside(segment.startPoint)
		|| IsInside(segment.endPoint);
}

bool StreetNode::IsInside(StreetSegment const& segment) const
{
	return IsInside(segment.startPoint)
		&& IsInside(segment.endPoint);
}

bool StreetNode::IsInside(glm::vec3 const& point) const
{
	//printf("X: %10f <= %10f <= %10f\n", this->_minPosition.x, point.x, this->_maxPosition.x);
	//printf("Z: %10f <= %10f <= %10f\n", this->_minPosition.y, point.z, this->_maxPosition.y);
	return point.x >= this->_minPosition.x
		&& point.x <= this->_maxPosition.x
		&& point.z >= this->_minPosition.y
		&& point.z <= this->_maxPosition.y;
}

StreetNode::RelativePosition StreetNode::RelativePositionTo(StreetSegment const& segment) const
{
	RelativePosition startPos = RelativePositionTo(segment.startPoint);
	RelativePosition endPos = RelativePositionTo(segment.endPoint);
	return startPos >= endPos ? startPos : endPos;
}

StreetNode::RelativePosition StreetNode::RelativePositionTo(glm::vec3 const& point) const
{
	auto result = int(LT);
	if (point.z <= _minPosition.y)
	{
		//	Bod leží dole, relativní pozice uzlu je nahoře
		result += 0;

		if (point.x <= _minPosition.x)
			//	Bod leží nalevo, relativní pozice uzlu je vpravo
			//	0 + 1 = RT
			result += 1;
		if (point.x >= _maxPosition.x)
			//	Bod leží napravo, relativní pozice uzlu je vlevo
			//	0 + 0 = LT
			result += 0;
	}
	else if (point.z >= _maxPosition.y)
	{
		//	Bod leží nahoře, relativní pozice uzlu je dole
		result += 2;

		if (point.x <= _minPosition.x)
			//	Bod leží nalevo, relativní pozice uzlu je vpravo
			//	2 + 0 = RB
			result += 0;
		if (point.x >= _maxPosition.x)
			//	Bod leží napravo, relativní pozice uzlu je vlevo
			//	2 + 0 = LB
			result += 1;
	}

	return static_cast<RelativePosition>(result);
}
