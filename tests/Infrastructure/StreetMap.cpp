#include <catch.hpp>
#include <glm/glm.hpp>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetMap.h>
#include <Infrastructure/StreetNode.h>


using namespace Infrastructure;

SCENARIO("Intersection, StreetSegment-StreetSegment [LT|Simple, Non-existent]", "[Infrastructure/StreetMap]")
{
	const StreetSegment segment1 = {
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(-1.f, 0.f, 1.f),
		glm::vec3(-1.f, 0.f, 0.f),
		1,
		nullptr
	};
	const StreetSegment segment2 = {
		glm::vec3(-2.f, 0.f, 0.f),
		glm::vec3(-2.f, 0.f, 2.f),
		glm::vec3(0.f, 0.f, 1.f),
		1,
		nullptr
	};

	auto intersection = StreetMap::Intersection(segment1, segment2);
	REQUIRE_FALSE(intersection.exists);
	REQUIRE(intersection.positionRelative.x == 2.f);
	REQUIRE(intersection.positionRelative.y == .5f);

}

SCENARIO("Intersection, StreetSegment-StreetSegment [RT|Simple, Non-existent]", "[Infrastructure/StreetMap]")
{
	//	RT intersectingSegment
	const StreetSegment segment1 = {
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 0.f),
		1,
		nullptr
	};
	const StreetSegment segment2 = {
		glm::vec3(2.f, 0.f, 0.f),
		glm::vec3(2.f, 0.f, 2.f),
		glm::vec3(0.f, 0.f, 1.f),
		1,
		nullptr
	};

	auto intersection = StreetMap::Intersection(segment1, segment2);
	REQUIRE_FALSE(intersection.exists);
	REQUIRE(intersection.positionRelative.x == 2.f);
	REQUIRE(intersection.positionRelative.y == .5f);
}

SCENARIO("Intersection, StreetSegment-StreetSegment [RB|Simple, Non-existent]", "[Infrastructure/StreetMap]")
{
	const StreetSegment segment1 = {
		glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(1.f, 0.f, -1.f),
		glm::vec3(1.f, 0.f, 0.f),
		1,
		nullptr
	};
	const StreetSegment segment2 = {
		glm::vec3(2.f, 0.f, 0.f),
		glm::vec3(2.f, 0.f, -2.f),
		glm::vec3(0.f, 0.f, -1.f),
		1,
		nullptr
	};

	auto intersection = StreetMap::Intersection(segment1, segment2);
	REQUIRE_FALSE(intersection.exists);
	REQUIRE(intersection.positionRelative.x == 2.f);
	REQUIRE(intersection.positionRelative.y == .5f);
}

SCENARIO("Intersection, StreetSegment-StreetSegment [LB|Simple, Non-existent]", "[Infrastructure/StreetMap]")
{
	const StreetSegment segment1 = {
		glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(-1.f, 0.f, -1.f),
		glm::vec3(-1.f, 0.f, 0.f),
		1,
		nullptr
	};
	const StreetSegment segment2 = {
		glm::vec3(-2.f, 0.f, 0.f),
		glm::vec3(-2.f, 0.f, -2.f),
		glm::vec3(0.f, 0.f, -1.f),
		1,
		nullptr
	};

	auto intersection = StreetMap::Intersection(segment1, segment2);
	REQUIRE_FALSE(intersection.exists);
	REQUIRE(intersection.positionRelative.x == 2.f);
	REQUIRE(intersection.positionRelative.y == .5f);
}

SCENARIO("Intersection, StreetSegment-StreetSegment [LT|Extended, Non-existent]", "[Infrastructure/StreetMap]")
{
	StreetSegment segment1 = {
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(-1.f, 0.f, 1.f),
		glm::vec3(-1.f, 0.f, 0.f),
		1,
		nullptr
	};
	StreetSegment segment2 = {
		glm::vec3(-1.f, 0.f, 0.f),
		glm::vec3(-3.f, 0.f, 2.f),
		glm::vec3(-.5f, 0.f, .5f),
		1,
		nullptr
	};

	auto intersection = StreetMap::Intersection(segment1, segment2);
	REQUIRE_FALSE(intersection.exists);
	REQUIRE(intersection.positionRelative.x == 2.f);
	REQUIRE(intersection.positionRelative.y == .5f);

	segment1 = {
		glm::vec3(-1.f, 0.f, 1.f),
		glm::vec3(-2.f, 0.f, 2.f),
		glm::vec3(-.5f, 0.f, .5f),
		1,
		nullptr
	};
	segment2 = {
		glm::vec3(-5.f, 0.f, 1.f),
		glm::vec3(-4.f, 0.f, 2.f),
		glm::vec3(.5f, 0.f, .5f),
		1,
		nullptr
	};

	intersection = StreetMap::Intersection(segment1, segment2);
	REQUIRE_FALSE(intersection.exists);
	//	TODO: Obráceně (hodnoty pro x místo y a naopak)??
	REQUIRE(intersection.positionRelative.x == 2.f);
	REQUIRE(intersection.positionRelative.y == 2.f);
}
