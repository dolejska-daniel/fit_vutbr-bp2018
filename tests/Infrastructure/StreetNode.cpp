#include <Infrastructure/StreetNode.h>
#include <glm/vec2.hpp>

#include <catch.hpp>


using namespace Infrastructure;

SCENARIO("Initialization #1 [Position, Size]")
{
	auto position = glm::vec2(0);
	StreetNode node(position, 4.f);

	REQUIRE(node.ReadPosition() == position);
	REQUIRE(node.ReadSize() == 4.f);
	REQUIRE(node.ReadMinPosition() == glm::vec2(-4.f, -4.f));
	REQUIRE(node.ReadMaxPosition() == glm::vec2(4.f, 4.f));
	REQUIRE(node.GetSegments().empty());
	REQUIRE(node.ReadSegments().empty());
	REQUIRE(node.GetChildren().empty());
	REQUIRE_FALSE(node.HasChildren());
}

SCENARIO("Initialization #2 [Position, Size, Empty Children]")
{
	auto position = glm::vec2(0);
	StreetNode node(position, 4.f, {});

	REQUIRE(node.ReadPosition() == position);
	REQUIRE(node.ReadSize() == 4.f);
	REQUIRE(node.ReadMinPosition() == glm::vec2(-4.f, -4.f));
	REQUIRE(node.ReadMaxPosition() == glm::vec2(4.f, 4.f));
	REQUIRE(node.GetSegments().empty());
	REQUIRE(node.ReadSegments().empty());
	REQUIRE(node.GetChildren().empty());
	REQUIRE_FALSE(node.HasChildren());
}

SCENARIO("Initialization #2 [Position, Size, One Child - pos1]")
{
	auto position = glm::vec2(0);
	const auto childNode = std::make_shared<StreetNode>(glm::vec2{position.x - 2.f, position.y + 2.f}, 2.f);
	StreetNode node(position, 4.f, { {0, childNode} });

	REQUIRE(node.ReadPosition() == position);
	REQUIRE(node.ReadSize() == 4.f);
	REQUIRE(childNode->ReadSize() == 2.f);
	REQUIRE(node.ReadMinPosition() == glm::vec2(-4.f, -4.f));
	REQUIRE(node.ReadMaxPosition() == glm::vec2(4.f, 4.f));
	REQUIRE(node.GetSegments().empty());
	REQUIRE(node.ReadSegments().empty());
	REQUIRE_FALSE(node.GetChildren().empty());
	REQUIRE(node.HasChildren());

	REQUIRE(node.GetChildren()[0] == childNode);
	REQUIRE(childNode->ReadPosition() == glm::vec2{ position.x - 2.f, position.y + 2.f });
	REQUIRE(childNode->ReadMinPosition() == glm::vec2(-4.f, 0.f));
	REQUIRE(childNode->ReadMaxPosition() == glm::vec2(0.f, 4.f));

	REQUIRE_FALSE(node.GetChildren()[1] == nullptr);
	REQUIRE(node.GetChildren()[1]->ReadPosition() == glm::vec2{ position.x + 2.f, position.y + 2.f });
	REQUIRE(node.GetChildren()[1]->ReadMinPosition() == glm::vec2(0.f, 0.f));
	REQUIRE(node.GetChildren()[1]->ReadMaxPosition() == glm::vec2(4.f, 4.f));

	REQUIRE_FALSE(node.GetChildren()[2] == nullptr);
	REQUIRE(node.GetChildren()[2]->ReadPosition() == glm::vec2{ position.x + 2.f, position.y - 2.f });
	REQUIRE(node.GetChildren()[2]->ReadMinPosition() == glm::vec2(0.f, -4.f));
	REQUIRE(node.GetChildren()[2]->ReadMaxPosition() == glm::vec2(4.f, 0.f));

	REQUIRE_FALSE(node.GetChildren()[3] == nullptr);
	REQUIRE(node.GetChildren()[3]->ReadPosition() == glm::vec2{ position.x - 2.f, position.y - 2.f });
	REQUIRE(node.GetChildren()[3]->ReadMinPosition() == glm::vec2(-4.f, -4.f));
	REQUIRE(node.GetChildren()[3]->ReadMaxPosition() == glm::vec2(0.f, 0.f));
}

SCENARIO("Initialization #2 [Position, Size, One Child - pos3]")
{
	auto position = glm::vec2(0);
	const auto childNode = std::make_shared<StreetNode>(glm::vec2{ position.x + 2.f, position.y - 2.f }, 2.f);
	StreetNode node(glm::vec2(0), 4.f, { {2, childNode} });

	REQUIRE(node.ReadPosition() == position);
	REQUIRE(node.ReadSize() == 4.f);
	REQUIRE(node.ReadMinPosition() == glm::vec2(-4.f, -4.f));
	REQUIRE(node.ReadMaxPosition() == glm::vec2(4.f, 4.f));
	REQUIRE(node.GetSegments().empty());
	REQUIRE(node.ReadSegments().empty());
	REQUIRE_FALSE(node.GetChildren().empty());

	REQUIRE(node.HasChildren());
	REQUIRE_FALSE(node.GetChildren()[0] == nullptr);
	REQUIRE(node.GetChildren()[0]->ReadPosition() == glm::vec2{ position.x - 2.f, position.y + 2.f });
	REQUIRE(node.GetChildren()[0]->ReadMinPosition() == glm::vec2(-4.f, 0.f));
	REQUIRE(node.GetChildren()[0]->ReadMaxPosition() == glm::vec2(0.f, 4.f));

	REQUIRE_FALSE(node.GetChildren()[1] == nullptr);
	REQUIRE(node.GetChildren()[1]->ReadPosition() == glm::vec2{ position.x + 2.f, position.y + 2.f });
	REQUIRE(node.GetChildren()[1]->ReadMinPosition() == glm::vec2(0.f, 0.f));
	REQUIRE(node.GetChildren()[1]->ReadMaxPosition() == glm::vec2(4.f, 4.f));

	REQUIRE(node.GetChildren()[2] == childNode);
	REQUIRE(childNode->ReadPosition() == glm::vec2{ position.x + 2.f, position.y - 2.f });
	REQUIRE(childNode->ReadMinPosition() == glm::vec2(0.f, -4.f));
	REQUIRE(childNode->ReadMaxPosition() == glm::vec2(4.f, 0.f));

	REQUIRE_FALSE(node.GetChildren()[3] == nullptr);
	REQUIRE(node.GetChildren()[3]->ReadPosition() == glm::vec2{ position.x - 2.f, position.y - 2.f });
	REQUIRE(node.GetChildren()[3]->ReadMinPosition() == glm::vec2(-4.f, -4.f));
	REQUIRE(node.GetChildren()[3]->ReadMaxPosition() == glm::vec2(0.f, 0.f));
}

SCENARIO("Initialization #2 [Position, Size, One Child - pos1 and pos3]")
{
	auto position = glm::vec2(0);
	const auto childNode1 = std::make_shared<StreetNode>(glm::vec2{ position.x - 2.f, position.y + 2.f }, 2.f);
	const auto childNode2 = std::make_shared<StreetNode>(glm::vec2{ position.x + 2.f, position.y - 2.f }, 2.f);
	StreetNode node(glm::vec2(0), 4.f, { {0, childNode1}, {2, childNode2} });

	REQUIRE(node.ReadPosition() == position);
	REQUIRE(node.ReadSize() == 4.f);
	REQUIRE(node.ReadMinPosition() == glm::vec2(-4.f, -4.f));
	REQUIRE(node.ReadMaxPosition() == glm::vec2(4.f, 4.f));
	REQUIRE(node.GetSegments().empty());
	REQUIRE(node.ReadSegments().empty());
	REQUIRE_FALSE(node.GetChildren().empty());

	REQUIRE(node.HasChildren());
	REQUIRE(node.GetChildren()[0] == childNode1);
	REQUIRE(childNode1->ReadPosition() == glm::vec2{ position.x - 2.f, position.y + 2.f });
	REQUIRE(childNode1->ReadMinPosition() == glm::vec2(-4.f, 0.f));
	REQUIRE(childNode1->ReadMaxPosition() == glm::vec2(0.f, 4.f));

	REQUIRE_FALSE(node.GetChildren()[1] == nullptr);
	REQUIRE(node.GetChildren()[1]->ReadPosition() == glm::vec2{ position.x + 2.f, position.y + 2.f });
	REQUIRE(node.GetChildren()[1]->ReadMinPosition() == glm::vec2(0.f, 0.f));
	REQUIRE(node.GetChildren()[1]->ReadMaxPosition() == glm::vec2(4.f, 4.f));

	REQUIRE(node.GetChildren()[2] == childNode2);
	REQUIRE(childNode2->ReadPosition() == glm::vec2{ position.x + 2.f, position.y - 2.f });
	REQUIRE(childNode2->ReadMinPosition() == glm::vec2(0.f, -4.f));
	REQUIRE(childNode2->ReadMaxPosition() == glm::vec2(4.f, 0.f));

	REQUIRE_FALSE(node.GetChildren()[3] == nullptr);
	REQUIRE(node.GetChildren()[3]->ReadPosition() == glm::vec2{ position.x - 2.f, position.y - 2.f });
	REQUIRE(node.GetChildren()[3]->ReadMinPosition() == glm::vec2(-4.f, -4.f));
	REQUIRE(node.GetChildren()[3]->ReadMaxPosition() == glm::vec2(0.f, 0.f));
}

SCENARIO("CreateParent [Isn't root]")
{
	auto node = StreetRootNode = std::make_shared<StreetNode>(glm::vec2(0), 4.f);
	REQUIRE_FALSE(node->IsRoot());
	REQUIRE_THROWS(node->CreateParent());
}

SCENARIO("CreateParent [Is root, LT]")
{
	auto node = StreetRootNode = std::make_shared<StreetNode>(glm::vec2(0), 4.f);
	StreetRootNode->SetRoot();
	REQUIRE(node->IsRoot());
	REQUIRE_NOTHROW(node->CreateParent(StreetNode::RelativePosition::LT));

	REQUIRE_FALSE(StreetRootNode == node);
	REQUIRE(StreetRootNode->IsRoot());
	REQUIRE_FALSE(node->IsRoot());

	REQUIRE(StreetRootNode->ReadSize() == 8.f);
	REQUIRE(StreetRootNode->ReadPosition() == glm::vec2(4.f, -4.f));
	REQUIRE(StreetRootNode->HasChildren());
	REQUIRE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::LT] == node);
}

SCENARIO("CreateParent [Is root, RT]")
{
	auto node = StreetRootNode = std::make_shared<StreetNode>(glm::vec2(0), 4.f);
	StreetRootNode->SetRoot();
	REQUIRE(node->IsRoot());
	REQUIRE_NOTHROW(node->CreateParent(StreetNode::RelativePosition::RT));

	REQUIRE_FALSE(StreetRootNode == node);
	REQUIRE(StreetRootNode->IsRoot());
	REQUIRE_FALSE(node->IsRoot());

	REQUIRE(StreetRootNode->ReadSize() == 8.f);
	REQUIRE(StreetRootNode->ReadPosition() == glm::vec2(-4.f, -4.f));
	REQUIRE(StreetRootNode->HasChildren());
	REQUIRE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::RT] == node);
}

SCENARIO("CreateParent [Is root, RB]")
{
	auto node = StreetRootNode = std::make_shared<StreetNode>(glm::vec2(0), 4.f);
	StreetRootNode->SetRoot();
	REQUIRE(node->IsRoot());
	REQUIRE_NOTHROW(node->CreateParent(StreetNode::RelativePosition::RB));

	REQUIRE_FALSE(StreetRootNode == node);
	REQUIRE(StreetRootNode->IsRoot());
	REQUIRE_FALSE(node->IsRoot());

	REQUIRE(StreetRootNode->ReadSize() == 8.f);
	REQUIRE(StreetRootNode->ReadPosition() == glm::vec2(-4.f, 4.f));
	REQUIRE(StreetRootNode->HasChildren());
	REQUIRE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::RB] == node);
}

SCENARIO("CreateParent [Is root, LB]")
{
	auto node = StreetRootNode = std::make_shared<StreetNode>(glm::vec2(0), 4.f);
	StreetRootNode->SetRoot();
	REQUIRE(node->IsRoot());
	REQUIRE_NOTHROW(node->CreateParent(StreetNode::RelativePosition::LB));

	REQUIRE_FALSE(StreetRootNode == node);
	REQUIRE(StreetRootNode->IsRoot());
	REQUIRE_FALSE(node->IsRoot());

	REQUIRE(StreetRootNode->ReadSize() == 8.f);
	REQUIRE(StreetRootNode->ReadPosition() == glm::vec2(4.f, 4.f));
	REQUIRE(StreetRootNode->HasChildren());
	REQUIRE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::LB] == node);
}

SCENARIO("CreateChildren [Empty]")
{
	auto position = glm::vec2(4.f);
	StreetNode node(position, 4.f);

	REQUIRE_FALSE(node.HasChildren());
	REQUIRE(node.GetChildren().empty());
	REQUIRE(node.GetChildren()[0] == nullptr);
	REQUIRE(node.GetChildren()[1] == nullptr);
	REQUIRE(node.GetChildren()[2] == nullptr);
	REQUIRE(node.GetChildren()[3] == nullptr);

	node.CreateChildren();
	REQUIRE(node.HasChildren());
	REQUIRE_FALSE(node.GetChildren().empty());

	REQUIRE_FALSE(node.GetChildren()[0] == nullptr);
	REQUIRE(node.GetChildren()[0]->ReadPosition() == glm::vec2{ position.x - 2.f, position.y + 2.f });
	REQUIRE(node.GetChildren()[0]->ReadMinPosition() == glm::vec2(0.f, 4.f));
	REQUIRE(node.GetChildren()[0]->ReadMaxPosition() == glm::vec2(4.f, 8.f));

	REQUIRE_FALSE(node.GetChildren()[1] == nullptr);
	REQUIRE(node.GetChildren()[1]->ReadPosition() == glm::vec2{ position.x + 2.f, position.y + 2.f });
	REQUIRE(node.GetChildren()[1]->ReadMinPosition() == glm::vec2(4.f, 4.f));
	REQUIRE(node.GetChildren()[1]->ReadMaxPosition() == glm::vec2(8.f, 8.f));

	REQUIRE_FALSE(node.GetChildren()[2] == nullptr);
	REQUIRE(node.GetChildren()[2]->ReadPosition() == glm::vec2{ position.x + 2.f, position.y - 2.f });
	REQUIRE(node.GetChildren()[2]->ReadMinPosition() == glm::vec2(4.f, 0.f));
	REQUIRE(node.GetChildren()[2]->ReadMaxPosition() == glm::vec2(8.f, 4.f));

	REQUIRE_FALSE(node.GetChildren()[3] == nullptr);
	REQUIRE(node.GetChildren()[3]->ReadPosition() == glm::vec2{ position.x - 2.f, position.y - 2.f });
	REQUIRE(node.GetChildren()[3]->ReadMinPosition() == glm::vec2(0.f, 0.f));
	REQUIRE(node.GetChildren()[3]->ReadMaxPosition() == glm::vec2(4.f, 4.f));
}

SCENARIO("CreateChildren [Non-empty]")
{
	const auto childNode0 = std::make_shared<StreetNode>(glm::vec2(-2), 4.f / 2);
	const auto childNode2 = std::make_shared<StreetNode>(glm::vec2(2), 4.f / 2);
	StreetNode node(glm::vec2(0), 4.f, { {0, childNode0}, {2, childNode2} });

	REQUIRE(node.HasChildren());
	REQUIRE(node.GetChildren()[0] == childNode0);
	REQUIRE_FALSE(node.GetChildren()[1] == nullptr);
	REQUIRE(node.GetChildren()[2] == childNode2);
	REQUIRE_FALSE(node.GetChildren()[3] == nullptr);

	const auto childNode1 = node.GetChildren()[1];
	const auto childNode3 = node.GetChildren()[3];

	node.CreateChildren();
	REQUIRE(node.HasChildren());
	REQUIRE(node.GetChildren()[0] == childNode0);
	REQUIRE(node.GetChildren()[1] == childNode1);
	REQUIRE(node.GetChildren()[2] == childNode2);
	REQUIRE(node.GetChildren()[3] == childNode3);
}

SCENARIO("Contains [Simple]")
{
	StreetNode node(glm::vec2(0), 4.f);
	const StreetSegment segment = {
		glm::vec3(-3.f, 0, -3.f),
		glm::vec3(3.f, 0, 3.f),
		glm::vec3(0.f),
	};

	REQUIRE_FALSE(node.Contains(segment));
	REQUIRE(node.Insert(segment));
	REQUIRE(node.Contains(segment));
	REQUIRE(node.Remove(segment));
	REQUIRE_FALSE(node.Contains(segment));
}

SCENARIO("Contains [Sub-node]")
{
	StreetNode node(glm::vec2(0), 4.f);
	const StreetSegment segment = {
		glm::vec3(1, 0, 3),
		glm::vec3(3, 0, 1),
		glm::vec3(0.f),
	};

	REQUIRE_FALSE(node.Contains(segment));
	REQUIRE(node.Insert(segment));
	REQUIRE_FALSE(node.Contains(segment));
	REQUIRE(node.ReadChildren()[StreetNode::RT]->Contains(segment));
}

SCENARIO("GetContainingNode [Simple]")
{
	auto node = std::make_shared<StreetNode>(glm::vec2(0), 4.f);
	const StreetSegment segment = {
		glm::vec3(-3.f, 0, -3.f),
		glm::vec3(3.f, 0, 3.f),
		glm::vec3(0.f),
	};

	REQUIRE(node->GetContainingNode(segment) == nullptr);
	REQUIRE(node->Insert(segment));
	REQUIRE(node->GetContainingNode(segment) == node);
}

SCENARIO("GetContainingNode [Sub-node]")
{
	auto node = std::make_shared<StreetNode>(glm::vec2(0), 4.f);
	const StreetSegment segment = {
		glm::vec3(1, 0, 3),
		glm::vec3(3, 0, 1),
		glm::vec3(0.f),
	};

	REQUIRE(node->GetContainingNode(segment) == nullptr);
	REQUIRE(node->Insert(segment));
	REQUIRE_FALSE(node->GetContainingNode(segment) == node);
	REQUIRE(node->GetContainingNode(segment) == node->GetChildren()[StreetNode::RT]);
}

SCENARIO("IsInside - One Segment [Contained]")
{
	const StreetNode node(glm::vec2(0), 4.f);
	const StreetSegment segment = {
		glm::vec3(1, 0, 3),
		glm::vec3(-2, 0, 1),
		glm::vec3(0.f),
	};

	REQUIRE(node.IsInside(segment));
}

SCENARIO("IsInside - One Segment [Partially contained]")
{
	const StreetNode node(glm::vec2(0), 4.f);
	const StreetSegment segment = {
		glm::vec3(3, 0, 2),
		glm::vec3(4, 0, 7),
		glm::vec3(0.f),
	};

	REQUIRE_FALSE(node.IsInside(segment));
}

SCENARIO("IsInside - One Segment [Not contained]")
{
	const StreetNode node(glm::vec2(0), 4.f);
	const StreetSegment segment = {
		glm::vec3(-6, 0, -13),
		glm::vec3(25, 0, 8),
		glm::vec3(0.f),
	};

	REQUIRE_FALSE(node.IsInside(segment));
}

SCENARIO("IsInside - Edges / Corners [Contained]")
{
	const StreetNode node(glm::vec2(0), 4.f);
	const std::vector<StreetSegment> segments = {
		{
			glm::vec3(-4, 0, 4),
			glm::vec3(-4, 0, -4),
			glm::vec3(0.f),
		},
		{
			glm::vec3(-4, 0, -4),
			glm::vec3(4, 0, -4),
			glm::vec3(0.f),
		},
		{
			glm::vec3(4, 0, -4),
			glm::vec3(4, 0, 4),
			glm::vec3(0.f),
		},
		{
			glm::vec3(4, 0, 4),
			glm::vec3(-4, 0, 4),
			glm::vec3(0.f),
		},
		{
			glm::vec3(-4, 0, 0),
			glm::vec3(0, 0, 0),
			glm::vec3(0.f),
		},
		{
			glm::vec3(4, 0, 0),
			glm::vec3(0, 0, 0),
			glm::vec3(0.f),
		},
		{
			glm::vec3(0, 0, -4),
			glm::vec3(0, 0, 0),
			glm::vec3(0.f),
		},
		{
			glm::vec3(0, 0, 4),
			glm::vec3(0, 0, 0),
			glm::vec3(0.f),
		}
	};

	for (auto const& segment : segments)
		REQUIRE(node.IsInside(segment));
}

SCENARIO("RelativePositionTo [Point]")
{
	const StreetNode node(glm::vec2(0), 4.f);

	REQUIRE(node.RelativePositionTo(glm::vec3( 5.f, 0, -8.f)) == StreetNode::RelativePosition::LT);
	REQUIRE(node.RelativePositionTo(glm::vec3(-5.f, 0, -8.f)) == StreetNode::RelativePosition::RT);
	REQUIRE(node.RelativePositionTo(glm::vec3(-5.f, 0,  8.f)) == StreetNode::RelativePosition::RB);
	REQUIRE(node.RelativePositionTo(glm::vec3( 5.f, 0,  8.f)) == StreetNode::RelativePosition::LB);
}

SCENARIO("RelativePositionTo [Segment, Simple]")
{
	const StreetNode node(glm::vec2(0), 4.f);

	StreetSegment segment = {
		glm::vec3(5.f, 0, -5.f),
		glm::vec3(8.f, 0, -8.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::LT);
	segment = {
		glm::vec3(-5.f, 0, -5.f),
		glm::vec3(-8.f, 0, -8.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::RT);
	segment = {
		glm::vec3(-5.f, 0, 5.f),
		glm::vec3(-8.f, 0, 8.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::RB);
	segment = {
		glm::vec3(5.f, 0, 5.f),
		glm::vec3(8.f, 0, 8.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::LB);
}

SCENARIO("RelativePositionTo [Segment, Extended]")
{
	const StreetNode node(glm::vec2(0), 4.f);

	//	Posuny po ose X
	StreetSegment segment = {
		glm::vec3(0.f, 0, -6.f),
		glm::vec3(6.f, 0, -6.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::LT);
	segment = {
		glm::vec3(0.f, 0, -6.f),
		glm::vec3(-6.f, 0, -6.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::RT);
	segment = {
		glm::vec3(0.f, 0, 6.f),
		glm::vec3(-6.f, 0, 6.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::RB);
	segment = {
		glm::vec3(0.f, 0, 6.f),
		glm::vec3(6.f, 0, 6.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::LB);

	//	Posuny po ose Y
	segment = {
		glm::vec3(6.f, 0, 0.f),
		glm::vec3(6.f, 0, -6.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::LT);
	segment = {
		glm::vec3(-6.f, 0, 0.f),
		glm::vec3(-6.f, 0, -6.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::RT);
	segment = {
		glm::vec3(-6.f, 0, 0.f),
		glm::vec3(-6.f, 0, 6.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::RB);
	segment = {
		glm::vec3(6.f, 0, 0.f),
		glm::vec3(6.f, 0, 6.f),
		glm::vec3(0.f),
	};
	REQUIRE(node.RelativePositionTo(segment) == StreetNode::RelativePosition::LB);
}

SCENARIO("Insert [Contained, whole]")
{
	StreetNode node(glm::vec2(0), 4.f);

	StreetSegment segment1 = {
		glm::vec3(4, 0, 1),
		glm::vec3(-3, 0, -2),
		glm::vec3(0.f),
	};
	REQUIRE(node.IsInside(segment1));
	REQUIRE(node.Insert(segment1));
	REQUIRE(node.GetSegments() == std::vector<StreetSegment>{segment1});
	REQUIRE(node.ReadSegments() == std::vector<StreetSegment>{segment1});
	REQUIRE(node.HasChildren());


	StreetSegment segment2 = {
		glm::vec3(-3, 0, 4),
		glm::vec3(3, 0, -4),
		glm::vec3(0.f),
	};
	REQUIRE(node.IsInside(segment2));
	REQUIRE(node.Insert(segment2));
	REQUIRE(node.GetSegments() == std::vector<StreetSegment>{segment1, segment2});
	REQUIRE(node.ReadSegments() == std::vector<StreetSegment>{segment1, segment2});
	REQUIRE(node.HasChildren());
}

SCENARIO("Insert [Contained, sub-node]")
{
	StreetNode node(glm::vec2(0), 4.f);

	StreetSegment segment1 = {
		glm::vec3(1, 0, 3),
		glm::vec3(3, 0, 1),
		glm::vec3(0.f),
	};
	REQUIRE(node.IsInside(segment1));
	REQUIRE(node.Insert(segment1));
	REQUIRE(node.ReadSegments().empty());

	REQUIRE_FALSE(node.ReadChildren()[0]->IsInside(segment1));
	REQUIRE(node.ReadChildren()[1]->IsInside(segment1));
	REQUIRE_FALSE(node.ReadChildren()[2]->IsInside(segment1));
	REQUIRE_FALSE(node.ReadChildren()[3]->IsInside(segment1));

	REQUIRE(node.ReadChildren()[1]->GetSegments() == std::vector<StreetSegment>{segment1});


	StreetSegment segment2 = {
		glm::vec3(-1, 0, -3),
		glm::vec3(-3, 0, -1),
		glm::vec3(0.f),
	};
	REQUIRE(node.IsInside(segment2));
	REQUIRE(node.Insert(segment2));
	REQUIRE(node.ReadSegments().empty());

	REQUIRE_FALSE(node.ReadChildren()[0]->IsInside(segment2));
	REQUIRE_FALSE(node.ReadChildren()[1]->IsInside(segment2));
	REQUIRE_FALSE(node.ReadChildren()[2]->IsInside(segment2));
	REQUIRE(node.ReadChildren()[3]->IsInside(segment2));

	REQUIRE(node.ReadChildren()[3]->GetSegments() == std::vector<StreetSegment>{segment2});

	StreetSegment segment3 = {
		glm::vec3(-3, 0, -1),
		glm::vec3(-1, 0, -3),
		glm::vec3(0.f),
	};
	REQUIRE(node.IsInside(segment3));
	REQUIRE(node.Insert(segment3));
	REQUIRE(node.ReadSegments().empty());

	REQUIRE_FALSE(node.ReadChildren()[0]->IsInside(segment3));
	REQUIRE_FALSE(node.ReadChildren()[1]->IsInside(segment3));
	REQUIRE_FALSE(node.ReadChildren()[2]->IsInside(segment3));
	REQUIRE(node.ReadChildren()[3]->IsInside(segment3));

	REQUIRE(node.ReadChildren()[3]->ReadSegments() == std::vector<StreetSegment>{segment2, segment3});
}

SCENARIO("Insert [Contained, sub-sub-node]")
{
	StreetNode node(glm::vec2(0), 4.f);

	StreetSegment segment1 = {
		glm::vec3(.5f, 0, 1.5f),
		glm::vec3(1.5f, 0, .5f),
		glm::vec3(0.f),
	};
	REQUIRE(node.IsInside(segment1));
	REQUIRE(node.Insert(segment1));
	REQUIRE(node.ReadSegments().empty());

	REQUIRE_FALSE(node.ReadChildren()[0]->IsInside(segment1));
	REQUIRE(node.ReadChildren()[1]->IsInside(segment1));
	REQUIRE_FALSE(node.ReadChildren()[2]->IsInside(segment1));
	REQUIRE_FALSE(node.ReadChildren()[3]->IsInside(segment1));
	REQUIRE(node.ReadChildren()[1]->ReadSegments().empty());

	REQUIRE(node.ReadChildren()[1]->HasChildren());
	REQUIRE_FALSE(node.ReadChildren()[1]->ReadChildren()[0]->IsInside(segment1));
	REQUIRE_FALSE(node.ReadChildren()[1]->ReadChildren()[1]->IsInside(segment1));
	REQUIRE_FALSE(node.ReadChildren()[1]->ReadChildren()[2]->IsInside(segment1));
	REQUIRE(node.ReadChildren()[1]->ReadChildren()[3]->IsInside(segment1));
	REQUIRE(node.ReadChildren()[1]->ReadChildren()[3]->ReadSegments() == std::vector<StreetSegment>{segment1});
}

SCENARIO("Insert [Not contained, No global root]")
{
	StreetNode node(glm::vec2(0), 4.f);

	StreetSegment segment = {
		glm::vec3(5.f, 0, 7.f),
		glm::vec3(7.f, 0, 5.f),
		glm::vec3(0.f),
	};
	REQUIRE_FALSE(node.IsInside(segment));
	//	Globální root není nastaven, vložení selže
	StreetRootNode = nullptr;
	REQUIRE_FALSE(node.Insert(segment));
}

SCENARIO("Insert [Parent-contained]")
{
	auto node = StreetRootNode = std::make_shared<StreetNode>(glm::vec2(0), 4.f);
	StreetRootNode->SetRoot();

	StreetSegment segment = {
		glm::vec3(0.f, 0, -7.f),
		glm::vec3(7.f, 0, 7.f),
		glm::vec3(0.f),
	};
	REQUIRE_FALSE(node->IsInside(segment));
	REQUIRE(node->Insert(segment));
	REQUIRE(node->ReadSegments().empty());

	//	Po vložení bude vytvořen nový root
	REQUIRE_FALSE(StreetRootNode == node);
	REQUIRE(StreetRootNode->IsInside(segment));
	REQUIRE(StreetRootNode->ReadSegments() == std::vector<StreetSegment>{segment});
}

SCENARIO("Insert [Parent-sub-contained, LB]")
{
	auto node = StreetRootNode = std::make_shared<StreetNode>(glm::vec2(0), 4.f);
	StreetRootNode->SetRoot();

	StreetSegment segment = {
		glm::vec3(-12.f, 0, -12.f),
		glm::vec3(-5.f, 0, -5.f),
		glm::vec3(0.f),
	};
	REQUIRE_FALSE(node->IsInside(segment));
	REQUIRE(node->Insert(segment));
	REQUIRE(node->ReadSegments().empty());

	//	Po vložení bude vytvořen nový root
	REQUIRE_FALSE(StreetRootNode == node);
	REQUIRE(StreetRootNode->IsInside(segment));
	REQUIRE(StreetRootNode->ReadSegments().empty());

	REQUIRE(StreetRootNode->HasChildren());
	REQUIRE_FALSE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::LT]->IsInside(segment));
	REQUIRE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::LT]->ReadSegments().empty());
	REQUIRE_FALSE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::RT]->IsInside(segment));
	REQUIRE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::RT]->ReadSegments().empty());
	REQUIRE_FALSE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::RB]->IsInside(segment));
	REQUIRE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::RB]->ReadSegments().empty());
	REQUIRE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::LB]->IsInside(segment));
	REQUIRE(StreetRootNode->ReadChildren()[StreetNode::RelativePosition::LB]->ReadSegments() == std::vector<StreetSegment>{segment});
}

SCENARIO("Remove [Simple]")
{
	StreetNode node(glm::vec2(0), 4.f);
	const StreetSegment segment = {
		glm::vec3(4, 0, 1),
		glm::vec3(-3, 0, -2),
		glm::vec3(0.f),
	};

	REQUIRE(node.Insert(segment));
	REQUIRE_FALSE(node.ReadSegments().empty());

	REQUIRE(node.Remove(segment));
	REQUIRE(node.ReadSegments().empty());
}

SCENARIO("Remove [Extended]")
{
	StreetNode node(glm::vec2(0), 4.f);
	const StreetSegment segment1 = {
		glm::vec3(4, 0, 1),
		glm::vec3(-3, 0, -2),
		glm::vec3(0.f),
	};
	const StreetSegment segment2 = {
		glm::vec3(-3, 0, -2),
		glm::vec3(4, 0, 1),
		glm::vec3(0.f),
	};
	const StreetSegment segment3 = {
		glm::vec3(-4, 0, -1),
		glm::vec3(3, 0, 2),
		glm::vec3(0.f),
	};
	const StreetSegment segment4 = {
		glm::vec3(3, 0, 2),
		glm::vec3(-4, 0, -1),
		glm::vec3(0.f),
	};

	REQUIRE(node.Insert(segment1));
	REQUIRE(node.Insert(segment2));
	REQUIRE(node.Insert(segment3));
	REQUIRE(node.Insert(segment4));
	REQUIRE(node.ReadSegments() == std::vector<StreetSegment>{segment1, segment2, segment3, segment4});

	REQUIRE(node.Remove(segment1));
	REQUIRE(node.ReadSegments() == std::vector<StreetSegment>{segment2, segment3, segment4});

	REQUIRE(node.Remove(segment3));
	REQUIRE(node.ReadSegments() == std::vector<StreetSegment>{segment2, segment4});

	REQUIRE(node.Remove(segment4));
	REQUIRE(node.ReadSegments() == std::vector<StreetSegment>{segment2});

	REQUIRE(node.Remove(segment2));
	REQUIRE(node.ReadSegments().empty());
}
