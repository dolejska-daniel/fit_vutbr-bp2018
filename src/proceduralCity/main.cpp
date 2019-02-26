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
#include <Infrastructure/StreetNode.h>
#include <algorithm>


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

	if (args.isPresent("--help", "") || args.validate() == false)
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
	Infrastructure::StreetRootNode = std::make_shared<Infrastructure::StreetNode>(vec2(0.f), 8.f);
	Infrastructure::StreetRootNode->SetRoot();

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

	auto streetMap = Infrastructure::StreetMap(map);
	//auto parcel = new Infrastructure::Parcel();
	std::vector<std::shared_ptr<Infrastructure::Parcel>> parcels;



	// ==========================================================dd=
	//	VYKRESLOVÁNÍ
	// =============================================================

	std::cerr << "Callback" << std::endl;
	//	Drawing
    mainLoop->setIdleCallback([&](){

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

			color = vec3(0, 1, 0);
			shaders->GetActiveProgram()->set3fv("color", &color[0]);

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

		for (auto i = 0; i < map->GetChunks().size(); i++)
		{
			auto chunk = map->GetChunk(i);
			if (chunk)
				renderer->Render(chunk);
		}

		shaders->Use("Phong");
		shaders->GetActiveProgram()->set3fv("lightPosition_worldspace", &cameraPosition[0]);
		shaders->GetActiveProgram()->set3fv("cameraPosition_worldspace", &cameraPosition[0]);

		if (KeyDown['x'])
		{
			streetMap.BuildStep();
		}

		static auto built = false;
		if (KeyDown['v'])
		{
			if (!built)
			{
				std::cerr << std::endl << "Building..." << std::endl;
				streetMap.BuildStep();
				built = true;
			}
		}
		else
			built = false;


		static auto changedRenderer = false;
		if (KeyDown['b'])
		{
			if (!changedRenderer)
			{
				render++;
				changedRenderer = true;
			}
		}
		else
			changedRenderer = false;

		static auto postprocessed = false;
		if (KeyDown['p'])
		{
			std::vector<std::shared_ptr<Infrastructure::Street>> nearby_streets;
			for (const auto& street1 : streetMap.GetStreets())
			{
				if (!street1->Ended())
					continue;
				if (std::find(nearby_streets.begin(), nearby_streets.end(), street1) != nearby_streets.end())
					// Již  přidáno
					continue;

				auto s1_segment_first = street1->GetSegment(0);
				auto s1_segment_last = street1->GetSegment();
				for (const auto& street2 : streetMap.GetStreets())
				{
					if (street1 == street2)
						// Tatáž ulice
						continue;
					if (!street2->Ended())
						// Pouze ukončené ulice
						continue;
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
					auto x  = glm::vec2{ s1_segment_first.direction.x, s1_segment_first.direction.z };
					auto x1 = glm::vec2{ s1_segment_first.startPoint.x, s1_segment_first.startPoint.z };
					auto y1 = glm::vec2{ s2_segment_first.startPoint.x, s2_segment_first.startPoint.z };
					auto d  = x1 - y1;
					auto xparallel = glm::dot(d, x) / glm::pow(glm::length(x), 2) * x;
					auto xperpendicular = d - xparallel;
					//--------------------------------------------------------------------

					if (glm::length(xperpendicular) < 4.f)
					{
						glm::vec3 direction_left{ -s2_segment_first.direction.z, s2_segment_first.direction.y, s2_segment_first.direction.x };
						glm::vec3 direction_right{ s2_segment_first.direction.z, s2_segment_first.direction.y, -s2_segment_first.direction.x };
						Infrastructure::StreetSegment first_segment =
						{
							s2_segment_first.startPoint + s2_segment_first.direction + direction_left * 8.f,
							s2_segment_first.startPoint + s2_segment_first.direction + direction_right * 8.f,
							glm::vec3(0),
							8.f,
						};
						auto intersection = Infrastructure::StreetMap::Intersection(first_segment, street1);
						if (!intersection.exists)
						{
							Infrastructure::StreetSegment last_segment =
							{
								s2_segment_last.endPoint - s2_segment_last.direction + direction_left * 8.f,
								s2_segment_last.endPoint - s2_segment_last.direction + direction_right * 8.f,
								glm::vec3(0),
								8.f,
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
					streetMap.RemoveStreet(street);
				}
				else
					renderer->Render(street);
			}

			postprocessed = true;
		}
		else
			postprocessed = false;

		static auto parcelGenerated = false;
		if (KeyDown['l'] && !parcelGenerated)
		{
			auto street = streetMap.GetStreets()[1];
			std::vector<std::shared_ptr<Infrastructure::Street>> visited;

			auto intersections = street->GetIntersections();
			if (!intersections.empty())
			{
				// Označení silnice za navštívenou
				visited.push_back(street);

				// Vytvoření nové parcely
				auto parcel = std::make_shared<Infrastructure::Parcel>();
				parcels.push_back(parcel);

				// Prvotní bod parcely
				parcel->AddBorderPoint(street->GetSegment(0).startPoint);

				// První křižovatka
				auto intersection = intersections.front();
				parcel->AddBorderPoint(intersection.point);

				// Dokud existují navazující silnice
				while (std::find(visited.begin(), visited.end(), intersection.street) == visited.end())
				{
					// Označení poslední ulice za navštívenou
					visited.push_back(intersection.street);

					// Následující křižovatky
					intersections = intersection.street->GetIntersections();
					if (!intersections.empty())
					{
						// Výběr křižovatky
						intersection = intersections.front();
						// Další bod parcely
						parcel->AddBorderPoint(intersection.point);
					}
					else
					{
						// Žádné další křiožvatky nejsou
						// Volba posledního bodu parcely
						auto point = intersection.street->GetSegment(0).startPoint;
						if (intersection.isSubstreet)
							point = intersection.street->GetSegment().endPoint;

						parcel->AddBorderPoint(point);
						break;
					}
				}
				// Uzavření parcely
				parcel->Finish();
			}
		}
		for (const auto& parcel : parcels)
			if (parcel->finished)
				renderer->Render(parcel);

		if (KeyDown['y'])
		{
			printf("Number of streets: %llu\n", streetMap.ReadStreets().size());
		}

		color = { 1, 1, 1 };
		shaders->GetActiveProgram()->set3fv("color", &color[0]);
		for (const auto& street : streetMap.ReadStreets())
		{
			renderer->Render(street);
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
