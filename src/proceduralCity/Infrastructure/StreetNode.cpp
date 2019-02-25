///
/// @file StreetNode.cpp
/// @brief Obsahuje definice funkcí pro třídu StreetNode.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#include <Infrastructure/StreetNode.h>
#include <Infrastructure/Street.h>
#include <glm/integer.hpp>
#include <algorithm>
#include <utility>



std::shared_ptr<Infrastructure::StreetNode> Infrastructure::StreetRootNode = nullptr;


using namespace Infrastructure;

StreetNode::StreetNode(glm::vec2 const& position, const float size, std::shared_ptr<StreetNode> parent)
	: _parent(std::move(parent)), _position(position), _size(size), _isRoot(false), _hasChildren(false)
{
	//	TODO: Možnost nastavovat root podle parent pointeru
	_minPosition = position - size;
	_maxPosition = position + size;
}

StreetNode::StreetNode(glm::vec2 const& position, const float size, std::map<int, std::shared_ptr<StreetNode>> const& children, std::shared_ptr<StreetNode> parent)
	: _parent(std::move(parent)), _position(position), _size(size), _isRoot(false), _hasChildren(false)
{
	//	Pro shared_from_this() musí existovat alespoň jeden aktivní shared_ptr
	auto ptr = std::shared_ptr<StreetNode>(this, [](StreetNode*){});

	//	TODO: Možnost nastavovat root podle parent pointeru
	_minPosition = position - size;
	_maxPosition = position + size;

	for (auto const& node : children)
	{
		_children[node.first] = node.second;
		if (node.second)
		{
			node.second->SetParent(shared_from_this());
			SetHasChildren(true);
		}
	}

	if (HasChildren())
		CreateChildren();
}

StreetNode::~StreetNode()
= default;


bool StreetNode::CreateChildren()
{
	glm::vec2 childPosition;
	const auto childSize = _size / 2.f;
	if (childSize <= .1f || childSize > _size)
		return false;

	// Left Top
	childPosition.x = _position.x - childSize;
	childPosition.y = _position.y + childSize;
	if (_children[LT] == nullptr)
		_children[LT] = std::make_shared<StreetNode>(childPosition, childSize, shared_from_this());

	// Right Top
	childPosition.x = _position.x + childSize;
	if (_children[RT] == nullptr)
		_children[RT] = std::make_shared<StreetNode>(childPosition, childSize, shared_from_this());

	// Right Bottom
	childPosition.y = _position.y - childSize;
	if (_children[RB] == nullptr)
		_children[RB] = std::make_shared<StreetNode>(childPosition, childSize, shared_from_this());

	// Left Bottom
	childPosition.x = _position.x - childSize;
	if (_children[LB] == nullptr)
		_children[LB] = std::make_shared<StreetNode>(childPosition, childSize, shared_from_this());

	SetHasChildren(true);
	return true;
}

bool StreetNode::CreateParent(RelativePosition const& oldRootPosition)
{
	if (StreetRootNode == nullptr)
		//	Kontrola existence uzlu na nejvyšší úrovni
		//	TODO: Silent fail pokud RootNode neexistuje?
		return false;

	if (!IsRoot())
		//	Pokus o vytvoření nadřazeného uzlu i přes to, že tento uzel není na nejvyšší úrovni
		throw std::logic_error("Only root node can create parent.");

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
	default:
		throw std::logic_error("Invalid old root relative position specified.");
	}

	//	Nový uzel nejvyšší úrovně
	StreetRootNode = std::make_shared<StreetNode>(position, size, children);
	StreetRootNode->SetRoot();
	//	Odpovídající nastavení tohoto uzlu
	this->SetRoot(false);
	this->SetParent(StreetRootNode);
	//	Vytvoření nadřazeného uzlu proběhlo úspěšně
	return true;
}

std::shared_ptr<StreetNode> StreetNode::GetContainingNode(StreetSegment const& segment)
{
	if (!HasInside(segment))
		//	Segment neleží v mezích uzlu
		return nullptr;

	if (Contains(segment))
		//	Uzel obsahuje daný segment
		return shared_from_this();

	if (HasChildren())
	{
		//	Uzel daný segment neobsahuje ale má podřízené uzly, které nejspíše ano
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
	if (HasInside(segment))
	{
		//	Koncový i počáteční bod je v mezích uzlu
		if (!HasChildren())
			//	Uzel nemá žádné podřízené uzly, pokusíme se je vytvořit
			CreateChildren();

		//	Nejdříve se pokusíme nalézt přesnější match, pokud je to možné
		auto node = _children.find(RelativePositionFor(segment));
		if (node != _children.end()) return node->second->Insert(segment);

		//	Vložení segmentu
		_segments.push_back(segment);
		return true;
	}
	
	if (IsRoot())
	{
		//	Tento uzel je nejvyšší existující, přesto segment nespadá do jeho mezí
		//	Proběhne vytvoření nadřazeného uzlu a pokus o vložení do něj
		if (CreateParent(RelativePositionTo(segment)))
			//	Pokus o vložení do nadřazeného uzlu
			return StreetRootNode->Insert(segment);
	}

	//	Segment není v mezí uzlu a zároveň uzel není na nejvyšší úrovni, vložení není možné
	return false;
}

bool StreetNode::Remove(StreetSegment const& segment)
{
	//	TODO: Specifikovat přesněji?
	if (!HasInside(segment))
		//	Segment neleží v mezích uzlu
		return false;

	if (!_segments.empty())
	{
		//	Segment leží v mezích uzlu a uzel obsahuje segmenty
		const auto r = std::find(_segments.begin(), _segments.end(), segment);
		if (r != _segments.end())
		{
			//	Segment byl nalezen
			_segments.erase(r);
			return true;
		}
	}

	if (HasChildren())
	{
		//	Tento uzel má podřízené uzly, které segment nejspíše obsahují
		auto node = _children.find(RelativePositionFor(segment));
		if (node != _children.end()) return node->second->Remove(segment);
	}

	return false;
}

bool StreetNode::Contains(StreetSegment const& segment) const
{
	const auto r = std::find(_segments.begin(), _segments.end(), segment);
	return r != _segments.end();
}

bool StreetNode::HasPartiallyInside(StreetSegment const& segment) const
{
	return HasInside(segment.startPoint)
		|| HasInside(segment.endPoint);
}

bool StreetNode::HasInside(StreetSegment const& segment) const
{
	return HasInside(segment.startPoint)
		&& HasInside(segment.endPoint);
}

bool StreetNode::HasInside(glm::vec3 const& point) const
{
	//printf("=== %p\n", this);
	//printf("X: %10f <= %10f <= %10f\n", this->_minPosition.x, point.x, this->_maxPosition.x);
	//printf("Z: %10f <= %10f <= %10f\n", this->_minPosition.y, point.z, this->_maxPosition.y);
	return point.x >= this->_minPosition.x
		&& point.x <= this->_maxPosition.x
		&& point.z >= this->_minPosition.y
		&& point.z <= this->_maxPosition.y;
}

StreetNode::RelativePosition StreetNode::RelativePositionTo(StreetSegment const& segment) const
{
	auto startPos = RelativePositionTo(segment.startPoint);
	auto endPos = RelativePositionTo(segment.endPoint);
	return startPos >= endPos ? startPos : endPos;
}

StreetNode::RelativePosition StreetNode::RelativePositionTo(glm::vec3 const& point) const
{
	if (point.z <= _minPosition.y)
	{
		//	Bod leží dole, relativní pozice uzlu je nahoře
		if (point.x <= _minPosition.x)
			//	Bod leží nalevo, relativní pozice uzlu je vpravo
			return RT;
		if (point.x >= _maxPosition.x)
			//	Bod leží napravo, relativní pozice uzlu je vlevo
			return LT;
	}
	else if (point.z >= _maxPosition.y)
	{
		//	Bod leží nahoře, relativní pozice uzlu je dole
		if (point.x <= _minPosition.x)
			//	Bod leží nalevo, relativní pozice uzlu je vpravo
			return RB;
		if (point.x >= _maxPosition.x)
			//	Bod leží napravo, relativní pozice uzlu je vlevo
			return LB;
	}
	else
	{
		//	Bod leží v Y mezích tohoto uzlu
		if (point.x <= _minPosition.x)
			//	Bod leží nalevo, relativní pozice uzlu je vpravo
			return RT;
		if (point.x >= _maxPosition.x)
			//	Bod leží napravo, relativní pozice uzlu je vlevo
			return LT;
	}

	return LT;
}

StreetNode::RelativePosition StreetNode::RelativePositionFor(StreetSegment const& segment) const
{
	const auto startPos = RelativePositionFor(segment.startPoint);
	const auto endPos = RelativePositionFor(segment.endPoint);
	if (startPos != endPos)
		//	Body se v rámci tohoto uzlu nenacházejí ve stejných částech
		return NONE;

	return startPos;
}

StreetNode::RelativePosition StreetNode::RelativePositionFor(glm::vec3 const& point) const
{
	if (point.x < _minPosition.x)
		//	Bod leží nalevo, mimo uzel
		return NONE;
	if (point.z > _maxPosition.y)
		//	Bod leží nad, mimo uzel
		return NONE;
	
	if (point.x <= _position.x)
	{
		//	Bod se nachází na levé polovině uzlu
		if (point.z >= _position.y)
			//	Bod se nachází v horní polovině
			return LT;
		if (point.z >= _minPosition.y)
			//	Bod se nachází v dolní polovině
			return LB;
	}
	else if (point.x <= _maxPosition.x)
	{
		//	Bod se nachází na pravé polovině uzlu
		if (point.z >= _position.y)
			//	Bod se nachází v horní polovině
			return RT;
		if (point.z >= _minPosition.y)
			//	Bod se nachází v dolní polovině
			return RB;
	}

	//	Bod neleží v mezích uzlu (napravo mimo/pod mimo)
	return NONE;
}
