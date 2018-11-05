#include <catch.hpp>
#include <glm/glm.hpp>
#include <Infrastructure/StreetMap.h>
#include <Infrastructure/StreetNode.h>


using namespace Infrastructure;

SCENARIO("Intersection, StreetSegment-StreetSegment [Simple, Non-existent]", "[Infrastructure/StreetMap]")
{
	//	RT segment
	StreetSegment segment1 = {
		glm::vec3(2.f, 0.f, 0.f),
		glm::vec3(2.f, 0.f, 2.f),
		glm::vec3(0.f, 0.f, 2.f),
		1,
		0
	};
	StreetSegment segment2 = {
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 0.f),
		1,
		0
	};

	auto intersection = StreetMap::Intersection(segment1, segment2);
	REQUIRE_FALSE(intersection.exists);
	//	TODO: Obráceně (hodnoty pro x místo y a naopak)??
	REQUIRE(intersection.positionRelative.x == .5f);
	REQUIRE(intersection.positionRelative.y == 2.f);

	//	LB segment
	segment1 = {
		glm::vec3(-2.f, 0.f, 0.f),
		glm::vec3(-2.f, 0.f, -2.f),
		glm::vec3(0.f, 0.f, -2.f),
		1,
		0
	};
	segment2 = {
		glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(-1.f, 0.f, -1.f),
		glm::vec3(-1.f, 0.f, 0.f),
		1,
		0
	};

	//	LT segment
	intersection = StreetMap::Intersection(segment1, segment2);
	REQUIRE_FALSE(intersection.exists);
	REQUIRE(intersection.positionRelative.x == .5f);
	REQUIRE(intersection.positionRelative.y == 2.f);

	segment1 = {
		glm::vec3(-2.f, 0.f, 0.f),
		glm::vec3(-2.f, 0.f, 2.f),
		glm::vec3(0.f, 0.f, 2.f),
		1,
		0
	};
	segment2 = {
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(-1.f, 0.f, 1.f),
		glm::vec3(-1.f, 0.f, 0.f),
		1,
		0
	};

	//	RB segment
	intersection = StreetMap::Intersection(segment1, segment2);
	REQUIRE_FALSE(intersection.exists);
	REQUIRE(intersection.positionRelative.x == .5f);
	REQUIRE(intersection.positionRelative.y == 2.f);

	segment1 = {
		glm::vec3(2.f, 0.f, 0.f),
		glm::vec3(2.f, 0.f, -2.f),
		glm::vec3(0.f, 0.f, -2.f),
		1,
		0
	};
	segment2 = {
		glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(1.f, 0.f, -1.f),
		glm::vec3(1.f, 0.f, 0.f),
		1,
		0
	};

	intersection = StreetMap::Intersection(segment1, segment2);
	REQUIRE_FALSE(intersection.exists);
	REQUIRE(intersection.positionRelative.x == .5f);
	REQUIRE(intersection.positionRelative.y == 2.f);
}

SCENARIO("Intersection, StreetSegment-StreetSegment [Extended, Non-existent]", "[Infrastructure/StreetMap]")
{

}
