///
/// @file main.cpp
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#define GLM_ENABLE_EXPERIMENTAL
#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2CPP/Window.h>
#include <SDL2CPP/MainLoop.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <BasicCamera/PerspectiveCamera.h>
#include <BasicCamera/FreeLookCamera.h>
#include <ArgumentViewer/ArgumentViewer.h>
#include <Vars/Vars.h>
#include <Terrain/Map.h>
#include <Terrain/Chunk.h>
#include <Terrain/Generator.h>
#include <Application/Application.h>
#include <Application/ShaderManager.h>
#include <Application/Renderer.h>
#include <Infrastructure/Parcel.h>
#include <Infrastructure/Street.h>
#include <Infrastructure/StreetMap.h>
#include <algorithm>
#include <Infrastructure/Building.h>
#include <Infrastructure/BuildingPart.h>
#include <Utils/QuadTree.h>
#include <Utils/functions.h>


using namespace glm;
using namespace ge::gl;
using namespace vars;
using namespace argumentViewer;
using namespace Application;
using namespace Terrain;

///
/// @brief
///
/// @param argc	number of arguments
/// @param argv	array of arguments
///
int main(const int argc, char* argv[])
{
	ArgumentViewer args(argc, argv);
	Vars vars;

	// ==========================================================dd=
	//	DEFINICE OBECNÝCH PROMĚNNÝCH A ARGUMENTŮ PROGRAMU
	// =============================================================
	vars.addString("resources.dir", args.gets("--resources-dir", "../res", "Path to resources directory (shaders, ...)"));
	vars.addString("output.dir",    args.gets("--output-dir", "../output", "Path to output directory (for saved models)"));

	// ==========================================================dd=
	//	DEFINICE PROMĚNNÝCH A ARGUMENTŮ PROGRAMU PRO NASTAVENÍ TERÉNU
	// =============================================================
	vars.addUint32("terrain.seed",			args.getu32("--terrain-seed",			12345,	"Default seed for terrain generation"));
	vars.addFloat( "terrain.scale",			args.getf32("--terrain-scale",			80.f,	"Perlin noise scale (higher the number smoother the changes)"));
	vars.addFloat( "terrain.amplitude",		args.getf32("--terrain-amplitude",		2.5f,	"Noise function amplitudes"));
	vars.addFloat( "terrain.frequency",		args.getf32("--terrain-frequency",		0.2f,	""));
	vars.addFloat( "terrain.persistence",	args.getf32("--terrain-persistence",	0.7f,	""));
	vars.addFloat( "terrain.lacunarity",	args.getf32("--terrain-lacunarity",		1.3f,	""));
	vars.addUint32("terrain.octaves",		args.getu32("--terrain-octaves",		3,		"Number of noises from which is the terrain going to be generated"));
	vars.addUint32("terrain.detail",		args.getu32("--terrain-detail",			1,		"Default level of object detail"));

	vars.addUint32("terrain.map.width",		args.getu32("--terrain-map-width",	2, "Terrain map width (count of chunks)"));
	vars.addUint32("terrain.map.height",	args.getu32("--terrain-map-height", 1, "Terrain map height (count of chunks) in 2D (depth/length in 3D)"));

	vars.addUint32("terrain.chunk.width",	args.getu32("--terrain-chunk-width",	64,		"Chunk width"));
	vars.addUint32("terrain.chunk.height",	args.getu32("--terrain-chunk-height",	64 ,		"Terrain chunk height in 2D (depth/length in 3D)"));
	vars.addFloat( "terrain.chunk.scale",	args.getf32("--terrain-chunk-scale",	8.f,	"Chunk scale multiplier"));

	if (args.isPresent("--help", "") || !args.validate())
	{
		std::cerr << args.toStr() << std::endl;
		exit(EXIT_SUCCESS);
	}


	// ==========================================================dd=
	//	INICIALIZACE OKNA, geGL, ...
	// =============================================================
	uint32_t width = 1024;
	uint32_t height = 768;

	//	Create main loop & window
	auto mainLoop = std::make_shared<sdl2cpp::MainLoop>();
	auto window   = std::make_shared<sdl2cpp::Window  >(width, height);
    window->createContext("rendering", 450u, sdl2cpp::Window::CORE, sdl2cpp::Window::DEBUG);
    mainLoop->addWindow("mainWindow", window);

	//	Initialize geGL
	init();

	// Enable debug mode
	setHighDebugMessage();
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	srand(vars.getUint32("terrain.seed"));

	std::cerr << "Using " << glGetString(GL_VERSION) << std::endl;


	// ==========================================================dd=
	//	DALŠÍ INICIALIZACE
	// =============================================================
	const auto v = float(vars.getUint32("terrain.map.width")) * float(vars.getUint32("terrain.chunk.width")) * vars.getFloat("terrain.chunk.scale");
	Utils::StreetQuadTree = std::make_shared<Utils::QuadTree>(Utils::RectBounds({ -v, -v }, { v * 2, v * 2 }));

	//	Initialize ShaderManager
	auto shaders = std::make_shared<ShaderManager>(vars);
	shaders->Use("Phong");

	auto renderer = std::make_shared<Renderer>(vars);
	auto color = vec3(0, 1, 0);

	//	Setup main camera
	auto cameraProjection = std::make_shared<basicCamera::PerspectiveCamera>(radians(45.f), width / height, 0.1f, INFINITY);
	//vars.add<std::shared_ptr<basicCamera::PerspectiveCamera>>("asdf", cameraProjection);
	auto freeLook = std::make_shared<basicCamera::FreeLookCamera>();
	freeLook->setPosition(vec3(1, 1, 4));

	const auto map = Generator::GenerateMap(vars);

	auto streetMap = std::make_shared<Infrastructure::StreetMap>(map);
	//auto parcel = new Infrastructure::Parcel();
	std::vector<std::shared_ptr<Infrastructure::Parcel>> parcels;
	std::vector<std::shared_ptr<Infrastructure::Building>> buildings;
	std::vector<std::shared_ptr<Infrastructure::Building>> streets;


	// ==========================================================dd=
	//	VYKRESLOVÁNÍ
	// =============================================================

	std::cerr << "Callback" << std::endl;
	//	Drawing
	mainLoop->setIdleCallback([&]() {

		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto a = 0; a < 3; a++)
			freeLook->move(a, float(KeyDown["d s"[a]] - KeyDown["acw"[a]]) * float(.25f + KeyDown[SDLK_LSHIFT] * 2.f));

		auto cameraPosition = freeLook->getPosition();
		auto projectionMatrix = cameraProjection->getProjection();
		auto viewMatrix = freeLook->getView();
		mat4 modelMatrix(1);

		static short render = 0;
		if (render == 0)
		{
			shaders->Use("Phong");

			shaders->GetActiveProgram()->set3fv("lightPosition_worldspace", &cameraPosition[0]);
			shaders->GetActiveProgram()->set3fv("cameraPosition_worldspace", &cameraPosition[0]);
		}
		else if (render == 1)
		{
			shaders->Use("Normal");
		}
		else
			render = 0;

		shaders->GetActiveProgram()->setMatrix4fv("projectionMatrix", &projectionMatrix[0][0]);
		shaders->GetActiveProgram()->setMatrix4fv("viewMatrix", &viewMatrix[0][0]);
		shaders->GetActiveProgram()->setMatrix4fv("modelMatrix", &modelMatrix[0][0]);

		if (render == 0)
			shaders->GetActiveProgram()->set3fv("color", &Utils::color_green[0]);
		for (const auto& chunk : map->GetChunks())
			if (chunk.second)
				renderer->Render(chunk.second);

		if (render == 0)
			shaders->GetActiveProgram()->set3fv("color", &Utils::color_red[0]);
		for (const auto& building : buildings)
			for (const auto& part : building->parts)
				renderer->Render(part);

		if (render == 0)
			shaders->GetActiveProgram()->set3fv("color", &Utils::color_black[0]);
		for (const auto& street : streets)
			for (const auto& part : street->parts)
				renderer->Render(part);


		shaders->Use("Phong");
		shaders->GetActiveProgram()->setMatrix4fv("projectionMatrix", &projectionMatrix[0][0]);
		shaders->GetActiveProgram()->setMatrix4fv("viewMatrix", &viewMatrix[0][0]);
		shaders->GetActiveProgram()->setMatrix4fv("modelMatrix", &modelMatrix[0][0]);

		shaders->GetActiveProgram()->set3fv("lightPosition_worldspace", &cameraPosition[0]);
		shaders->GetActiveProgram()->set3fv("cameraPosition_worldspace", &cameraPosition[0]);

		if (KeyDown['x'])
		{
			streetMap->BuildStep();
			std::cerr << "Number of streets: " << streetMap->ReadStreets().size() << std::endl;
		}

		static auto built = false;
		if (KeyDown['y'])
		{
			if (!built)
			{
				std::cerr << std::endl << "Building..." << std::endl;
				std::cerr << "Number of streets: " << streetMap->ReadStreets().size() << std::endl;

				streetMap->BuildStep();
				built = true;
			}
		}
		else
			built = false;


		static auto changedRenderer = false;
		static auto debugRender = true;
		if (KeyDown['r'])
		{
			if (KeyDown[SDLK_LCTRL])
			{
				if (!changedRenderer)
				{
					debugRender = !debugRender;
				}
			}
			else if (!changedRenderer)
			{
				render++;
			}

			changedRenderer = true;
		}
		else
			changedRenderer = false;


		static auto _m = false;
		if (KeyDown['m'])
		{
			if (!_m)
			{
				streetMap->ValidateIntersections();
				_m = true;
			}
		}
		else
			_m = false;


		static size_t parcel_id = 0;
		static auto generateBuilding = false;
		if (KeyDown['b'])
		{
			if (!generateBuilding)
			{
				if (KeyDown[SDLK_LCTRL])
				{
					while (parcel_id < parcels.size())
					{
						if (parcels[parcel_id]->type != Infrastructure::ParcelType::BUILDING)
							continue;

						auto building = std::make_shared<Infrastructure::Building>(parcels[parcel_id], map->GetHeightMap(), Infrastructure::SQUARE);
						buildings.emplace_back(building);

						parcel_id++;
					}
				}
				else if (KeyDown['o'])
				{
					buildings.clear();
					parcel_id = 0;
				}
				else
				{
					while (parcel_id < parcels.size() && parcels[parcel_id]->type != Infrastructure::ParcelType::BUILDING)
						parcel_id++;

					if (parcel_id < parcels.size())
					{
						auto building = std::make_shared<Infrastructure::Building>(parcels[parcel_id], map->GetHeightMap(), Infrastructure::SQUARE);
						buildings.emplace_back(building);
						parcel_id++;
					}
				}

				if (!KeyDown[SDLK_LSHIFT])
					generateBuilding = true;
			}
		}
		else
			generateBuilding = false;

		static size_t street_parcel_id = 0;
		static auto generateStreet = false;
		if (KeyDown['n'])
		{
			if (!generateStreet)
			{
				if (KeyDown['o'])
				{
					streets.clear();
				}
				else
				{
					while (street_parcel_id < parcels.size() && parcels[street_parcel_id]->type != Infrastructure::ParcelType::STREET)
						street_parcel_id++;

					if (street_parcel_id < parcels.size())
					{
						auto street = std::make_shared<Infrastructure::Building>(parcels[street_parcel_id], map->GetHeightMap(), Infrastructure::STREET_SQUARE);
						streets.emplace_back(street);
						street_parcel_id++;
					}
				}

				if (!KeyDown[SDLK_LSHIFT])
					generateStreet = true;
			}
		}
		else
			generateStreet = false;

		static auto postprocessed = false;
		if (KeyDown['p'])
		{
			std::vector<std::shared_ptr<Infrastructure::Street>> nearby_streets;
			for (const auto& street1 : streetMap->GetStreets())
			{
				if (!street1->Ended())
					continue;
				if (std::find(nearby_streets.begin(), nearby_streets.end(), street1) != nearby_streets.end())
					// Již  přidáno
					continue;

				auto s1_segment_first = street1->GetSegment(0);
				auto s1_segment_last = street1->GetSegment();
				for (const auto& street2 : streetMap->GetStreets())
				{
					if (street1 == street2)
						// Tatáž ulice
						continue;
					/*
					if (!street2->Ended())
						// Pouze ukončené ulice
						continue;*/
					if (std::find(nearby_streets.begin(), nearby_streets.end(), street2) != nearby_streets.end())
						// Již  přidáno
						continue;

					auto s2_segment_first = street2->GetSegment(0);
					auto s2_segment_last = street2->GetSegment();

					if (s1_segment_first.direction == -s2_segment_last.direction
						&& s1_segment_last.direction == -s2_segment_first.direction)
					{
						// Rovnoběžné, opačný směr
					}
					else if (s1_segment_first.direction == s2_segment_last.direction
						&& s1_segment_last.direction == s2_segment_first.direction)
					{
						// Rovnoběžné, stejný směr
					}
					else
						continue;

					//--------------------------------------------------------------------
					// https://math.stackexchange.com/a/210865
					//
					auto x = glm::vec2{ s1_segment_first.direction.x, s1_segment_first.direction.z };
					auto x1 = glm::vec2{ s1_segment_first.startPoint.x, s1_segment_first.startPoint.z };
					auto y1 = glm::vec2{ s2_segment_first.startPoint.x, s2_segment_first.startPoint.z };
					auto d = x1 - y1;
					auto xparallel = glm::dot(d, x) / glm::pow(glm::length(x), 2) * x;
					auto xperpendicular = d - xparallel;
					//--------------------------------------------------------------------

					const auto dist_max = 20.f;
					if (glm::length(xperpendicular) < dist_max)
					{
						glm::vec3 direction_left{ -s2_segment_first.direction.z, s2_segment_first.direction.y, s2_segment_first.direction.x };
						glm::vec3 direction_right{ s2_segment_first.direction.z, s2_segment_first.direction.y, -s2_segment_first.direction.x };
						Infrastructure::StreetSegment first_segment =
						{
							s2_segment_first.startPoint + s2_segment_first.direction + direction_left * dist_max * 2.f,
							s2_segment_first.startPoint + s2_segment_first.direction + direction_right * dist_max * 2.f,
							glm::vec3(0),
							dist_max * 2.f,
						};
						auto intersection = Infrastructure::StreetMap::Intersection(first_segment, street1);
						if (!intersection.exists)
						{
							Infrastructure::StreetSegment last_segment =
							{
								s2_segment_last.endPoint - s2_segment_last.direction + direction_left * dist_max * 2.f,
								s2_segment_last.endPoint - s2_segment_last.direction + direction_right * dist_max * 2.f,
								glm::vec3(0),
								dist_max * 2.f,
							};
							intersection = Infrastructure::StreetMap::Intersection(last_segment, street1);
							if (!intersection.exists)
								continue;
						}

						nearby_streets.push_back(street2);
					}
				}
			}

			std::cerr << "Nearby streets found: " << nearby_streets.size() << std::endl;

			color = { 1, 0, 0 };
			shaders->GetActiveProgram()->set3fv("color", &color[0]);
			for (const auto& street : nearby_streets)
			{
				if (KeyDown['o'])
				{
					streetMap->RemoveStreet(street);
				}
				else
					renderer->Render(street);
			}

			postprocessed = true;
		}
		else
			postprocessed = false;


		static size_t intersection_id = 0;
		static size_t street_id = 0;
		if (KeyDown['h'])
		{
			intersection_id = 0;
			street_id = 0;
			parcels.clear();
		}

		if (KeyDown['j'])
		{
			std::vector<std::shared_ptr<Infrastructure::Parcel>> parcels_invalid;

			for (const auto& parcel : parcels)
				if (parcel->GetBorderPoints().size() != 4)
					parcels_invalid.push_back(parcel);

			for (const auto& parcel : parcels_invalid)
				parcels.erase(std::remove(parcels.begin(), parcels.end(), parcel), parcels.end());
		}


		static std::vector<Infrastructure::StreetNarrowPair> visited_pairs;
		std::vector<std::shared_ptr<Infrastructure::Street>> visited;
		std::shared_ptr<Infrastructure::Parcel> parcel;
		std::function<void(std::shared_ptr<Infrastructure::Street>, Infrastructure::StreetSegment, glm::vec3, Infrastructure::StreetNarrowPair, Infrastructure::StreetIntersectionSide)> processStreet2 =
			[&](const std::shared_ptr<Infrastructure::Street>& street, const Infrastructure::StreetSegment& segment, const glm::vec3& point_from, Infrastructure::StreetNarrowPair const& previous_pair, const Infrastructure::StreetIntersectionSide side)
		{
			/*
			std::cerr << "processStreet2" << std::endl;
			std::cerr << "Street: " << street.get() << std::endl;
			*/

			if (!street || std::find(visited.begin(), visited.end(), street) != visited.end())
				return;
			/*
			std::cerr << "I     : " << street->GetIntersections().size() << std::endl;
			std::cerr << "IL    : " << street->GetLeftIntersectionPointPairs().size() << std::endl;
			std::cerr << "IR    : " << street->GetRightIntersectionPointPairs().size() << std::endl;
			*/


			Infrastructure::StreetNarrowPair pair;
			if (street->GetNextIntersectionPointPair(previous_pair, segment, side, &pair))
			{
				if (std::find_if(visited_pairs.begin(), visited_pairs.end(), [&pair](const Infrastructure::StreetNarrowPair& p)
				{
					return glm::distance(p.point1, pair.point1) < 1.f && glm::distance(p.point2, pair.point2) < 1.f
						|| glm::distance(p.point2, pair.point1) < 1.f && glm::distance(p.point1, pair.point2) < 1.f;
				}) != visited_pairs.end())
				{
					return;
				}
				visited_pairs.push_back(pair);

				// byla nalezena navazující dvojice křižovatek
				parcel->AddBorderPoint(pair.point1);
				if (!parcel->AddBorderPoint(pair.point2))
					return;

				visited.push_back(street);
				auto street_next = pair.intersection2.intersecting_segment.street;
				auto segment_next = pair.intersection2.intersecting_segment;
				if (street == street_next)
				{
					street_next = pair.intersection2.own_segment.street;
					segment_next = pair.intersection2.own_segment;
				}
				processStreet2(street_next, segment_next, pair.point2, pair, side);
			}
			else
			{
				//std::cerr << "Next pair was not found, using end point." << std::endl;
				// žádná další dvojice křižovatek nalezena nebyla
				parcel->AddBorderPoint(street->GetSegment().endPoint);
			}
		};

		std::function<void(std::shared_ptr<Infrastructure::Street>, glm::vec3, Infrastructure::StreetIntersectionSide)> processStreet =
			[&](const std::shared_ptr<Infrastructure::Street>& street, const glm::vec3& point_from, const Infrastructure::StreetIntersectionSide side)
		{
			/*
			std::cerr << "processStreet" << std::endl;
			std::cerr << "Street: " << street.get() << std::endl;
			*/

			if (std::find(visited.begin(), visited.end(), street) != visited.end())
				return;
			/*
			std::cerr << "I     : " << street->GetIntersections().size() << std::endl;
			std::cerr << "IL    : " << street->GetLeftIntersectionPointPairs().size() << std::endl;
			std::cerr << "IR    : " << street->GetRightIntersectionPointPairs().size() << std::endl;
			*/


			Infrastructure::StreetNarrowPair pair;
			if (street->GetNextIntersectionPointPair(point_from, side, side, false, &pair))
			{
				if (std::find_if(visited_pairs.begin(), visited_pairs.end(), [&pair](const Infrastructure::StreetNarrowPair& p)
				{
					return glm::distance(p.point1, pair.point1) < 1.f && glm::distance(p.point2, pair.point2) < 1.f
						|| glm::distance(p.point2, pair.point1) < 1.f && glm::distance(p.point1, pair.point2) < 1.f;
				}) != visited_pairs.end())
				{
					return;
				}
				visited_pairs.push_back(pair);

				// byla nalezena navazující dvojice křižovatek
				parcel->AddBorderPoint(pair.point1);
				if (!parcel->AddBorderPoint(pair.point2))
					return;

				visited.push_back(street);
				auto street_next = pair.intersection2.intersecting_segment.street;
				auto segment_next = pair.intersection2.intersecting_segment;
				if (street == street_next)
				{
					street_next = pair.intersection2.own_segment.street;
					segment_next = pair.intersection2.own_segment;
				}
				processStreet2(street_next, segment_next, pair.point2, pair, side);
			}
			else
			{
				//std::cerr << "Next pair was not found, using end point." << std::endl;
				// žádná další dvojice křižovatek nalezena nebyla
				parcel->AddBorderPoint(street->GetSegment().endPoint);
			}
		};

		static auto parcelGenerated = false;
		if (KeyDown['k'])
		{
			if (!parcelGenerated)
			{
				for (const auto& street : streetMap->GetStreets())
					street->GenerateIntersectionPointLists();

				const auto resize_factor = 1.f;

				std::function<void(std::shared_ptr<Infrastructure::Street>)> processStreetToList =
					[&](const std::shared_ptr<Infrastructure::Street>& street)
				{
					for (const auto& pair : street->GetLeftIntersectionPointPairs())
					{
						//std::cerr << std::endl << "New left parcel..." << std::endl;
						parcel = std::make_shared<Infrastructure::Parcel>();
						parcels.push_back(parcel);
						processStreet(street, pair.point1, Infrastructure::LEFT);
						//std::cerr << "Finishing parcel." << std::endl;
						parcel->Shrink(resize_factor);
						visited.clear();
					}

					for (const auto& pair : street->GetRightIntersectionPointPairs())
					{
						//std::cerr << std::endl << "New right parcel..." << std::endl;
						parcel = std::make_shared<Infrastructure::Parcel>();
						parcels.push_back(parcel);
						processStreet(street, pair.point1, Infrastructure::RIGHT);
						//std::cerr << "Finishing parcel." << std::endl;
						parcel->Shrink(resize_factor);
						visited.clear();
					}
				};

				for (const auto& street : streetMap->GetStreets())
				{
					processStreetToList(street);

					auto createStreet = [&parcel,&parcels,&resize_factor](glm::vec3 p1, glm::vec3 p2)
					{
						//auto midpoint = (p1 + p2) / 2.f;
						//p1 += glm::normalize(midpoint - p1) * resize_factor;
						//p2 += glm::normalize(midpoint - p2) * resize_factor;

						parcel = std::make_shared<Infrastructure::Parcel>(Infrastructure::STREET);
						parcels.push_back(parcel);

						const auto dir   = glm::normalize(p2 - p1);
						const auto left  = Utils::left_vector(dir);
						const auto right = Utils::right_vector(dir);

						auto street_resize_factor = resize_factor * .7f;
						parcel->AddBorderPoint(p1 + left * street_resize_factor);
						parcel->AddBorderPoint(p1 + right * street_resize_factor);
						parcel->AddBorderPoint(p2 + right * street_resize_factor);
						parcel->AddBorderPoint(p2 + left * street_resize_factor);
						parcel->Finish();
					};

					std::cerr << "Creating street parcel..." << std::endl;
					auto point1 = street->GetSegmentPoint(0, 0.f);
					auto point2 = point1;
					for (auto& intersection : street->GetIntersections())
					{
						point1 = point2;
						point2 = intersection.point;

						createStreet(point1, point2);
					}

					point1 = point2;
					point2 = street->GetSegmentPoint(1.f);

					createStreet(point1, point2);
				}
			}

			parcelGenerated = true;
		}
		else if (KeyDown['l'])
		{
			if (!parcelGenerated)
			{
				for (const auto& street : streetMap->GetStreets())
					street->GenerateIntersectionPointLists();

				if (street_id >= streetMap->GetStreets().size())
					return;

				auto street_current = streetMap->GetStreets()[street_id];
				auto rightPairs = street_current->GetRightIntersectionPointPairs();
				if (intersection_id < rightPairs.size())
				{
					auto pair = rightPairs[intersection_id];

					std::cerr << std::endl << "New right parcel..." << std::endl;
					parcel = std::make_shared<Infrastructure::Parcel>();
					parcels.push_back(parcel);
					processStreet(street_current, pair.point1, Infrastructure::RIGHT);
					std::cerr << "Finishing parcel." << std::endl;
					parcel->Shrink(1.f);
					parcel->Finish();

					visited.clear();
				}

				auto leftPairs = street_current->GetLeftIntersectionPointPairs();
				if (intersection_id < leftPairs.size())
				{
					auto pair = leftPairs[intersection_id];

					std::cerr << std::endl << "New left parcel..." << std::endl;
					parcel = std::make_shared<Infrastructure::Parcel>();
					parcels.push_back(parcel);
					processStreet(street_current, pair.point1, Infrastructure::LEFT);
					std::cerr << "Finishing parcel." << std::endl;
					parcel->Shrink(1.f);
					parcel->Finish();

					visited.clear();
				}

				if (intersection_id >= rightPairs.size()
					&& intersection_id >= leftPairs.size())
				{
					intersection_id = 0;
					street_id++;
				}
				else
				{		
					intersection_id++;
				}
			}

			parcelGenerated = true;
		}
		else
			parcelGenerated = false;

		static auto save = false;
		if (KeyDown[SDLK_LCTRL])
		{
			if (!save)
			{
				if (KeyDown['s'])
				{
					auto terrain_fp = vars.getString("output.dir") + "/terrain.obj";
					auto terrain_f = Utils::write_file(terrain_fp);

					std::cerr << "Saving terrain model to: " << terrain_fp << std::endl;
					renderer->Save(map, terrain_f);

					auto building_fp = terrain_fp;
					auto building_f = std::move(terrain_f);
					if (!KeyDown[SDLK_LSHIFT])
					{
						building_fp = vars.getString("output.dir") + "/buildings.obj";
						building_f = Utils::write_file(building_fp);
					}
					std::cerr << "Saving building models to: " << building_fp << std::endl;
					renderer->Save(buildings, building_f);

					save = true;
				}
			}
			else
				save = false;
		}
		else
			save = false;

		if (debugRender)
		{
			for (const auto& p : parcels)
			{
				if (p->finished)
				{
					if (p->type == Infrastructure::BUILDING)
					{
						color = Utils::color_blue;
						shaders->GetActiveProgram()->set3fv("color", &color[0]);
					}
					else if (p->type == Infrastructure::STREET)
					{
						color = Utils::color_rgb(140, 0, 255);
						shaders->GetActiveProgram()->set3fv("color", &color[0]);
					}
					renderer->Render(p);
				}
			}

			color = Utils::color_white;
			shaders->GetActiveProgram()->set3fv("color", &color[0]);
			for (const auto& s : streetMap->ReadStreets())
				if (!s->Destroyed())
					renderer->Render(s);
		}

		glFinish();
        window->swap();
    });

	//	Camera rotation
	window->setEventCallback(SDL_MOUSEMOTION, [&](SDL_Event const& event) {
		if (event.motion.state & SDL_BUTTON_LMASK)
		{
			//	Trap mouse
			SDL_SetRelativeMouseMode(SDL_TRUE);
			
			auto const sensitivity = 0.0025f;
			auto const xrel = static_cast<float>(event.motion.xrel);
			auto const yrel = static_cast<float>(event.motion.yrel);

			freeLook->setAngle(1, freeLook->getAngle(1) + xrel * sensitivity);
			freeLook->setAngle(0, freeLook->getAngle(0) + yrel * sensitivity);

			return true;
		}

		//	Free mouse
		SDL_SetRelativeMouseMode(SDL_FALSE);
		return false;
	});

	window->setEventCallback(SDL_KEYDOWN, [&](SDL_Event const& event) {
		KeyDown[event.key.keysym.sym] = true;
		return true;
	});

	window->setEventCallback(SDL_KEYUP, [&](SDL_Event const& event) {
		KeyDown[event.key.keysym.sym] = false;
		return true;
	});

	window->setWindowEventCallback(SDL_WINDOWEVENT_RESIZED, [&](SDL_Event const& event) {
		width = event.window.data1;
		height = event.window.data2;
		glViewport(0, 0, width, height);
		cameraProjection->setAspect(float(width / height));
		return true;
	});

	//	Start main loop
	(*mainLoop)();

    return EXIT_SUCCESS;
}
