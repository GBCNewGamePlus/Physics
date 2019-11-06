#define STB_IMAGE_IMPLEMENTATION
#include "RenderingSystem.h"
#include "InputEventSystem.h"
#include "RotateSystem.h"
#include "ParticleSystem.h"
#include "ParticleSpawnerSystem.h"
#include "GravityForceGeneratorSystem.h"
#include "FixedSpringForceGeneratorSystem.h"
#include "ForceAccumulatorSystem.h"
#include "PairedSpringForceGeneratorSystem.h"
#include "FPSControlSystem.h"
#include "SphereComponent.h"
/* Bungee */
#include "BungeeSpawnComponent.h"
#include "BungeeSpawnSystem.h"
#include "BungeeCordForceGeneratorSystem.h"
#include "BungeeCordComponent.h"
/* Buoyancy */
#include "BuoyancyComponent.h"
#include "BuoyancyForceGeneratorSystem.h"
#include "BuoyancySpawnComponent.h"
#include "BuoyancySpawnSystem.h"
#include "BuoyancyDensitySystem.h"
/* NBody */
#include "NBodyComponent.h"
#include "NBodyForceGeneratorSystem.h"

#define DEBUG_LOG_LEVEL 3

using namespace Reality;

void LoadAssets(ECSWorld& world);
//void MakeABunchaSprings(ECSWorld& world);
void MakeABunchaPlanets(ECSWorld& world);
void MakeBungeeChordSeed(ECSWorld& world);
void MakeBucket(ECSWorld& world);

int main()
{
	ECSWorld world;

	// Init and Load
	world.data.InitRendering();
	LoadAssets(world);
	
	world.data.renderUtil->camera.Position = Vector3(0, -7.5f, 5);

	// Create entities

	// Make a player controller
	auto e = world.createEntity();
	e.addComponent<FPSControlComponent>();
	
	//auto wall = world.createEntity();
	//wall.addComponent<TransformComponent>(Vector3(0, -15.0f, 15.0f), Vector3(3.0f, 3.0f, 3.0f), Vector3(0, 180, 0));
	//// Add mesh
	//wall.addComponent<MeshComponent>(
		//"Resources/Models/animeclassroom/anime school.obj",
		//"Shaders/vertexDefault.vs",
		//"Shaders/fragmentDefault.fs");

	// Uncomment for part 1 
	//MakeBungeeChordSeed(world);

	// Uncomment for part 2
	//MakeBucket(world);

	// Uncomment for part 3
	MakeABunchaPlanets(world);
	// Create Systems
	world.getSystemManager().addSystem<RenderingSystem>();
	world.getSystemManager().addSystem<InputEventSystem>();
	world.getSystemManager().addSystem<RotateSystem>();
	world.getSystemManager().addSystem<ParticleSystem>();
	world.getSystemManager().addSystem<ParticleSpawnerSystem>();
	world.getSystemManager().addSystem<GravityForceGeneratorSystem>();
	world.getSystemManager().addSystem<FixedSpringForceGeneratorSystem>();
	world.getSystemManager().addSystem<PairedSpringForceGeneratorSystem>();
	world.getSystemManager().addSystem<NBodyForceGeneratorSystem>();
	world.getSystemManager().addSystem<ForceAccumulatorSystem>();
	world.getSystemManager().addSystem<FPSControlSystem>();
	world.getSystemManager().addSystem<BungeeSpawnSystem>();
	world.getSystemManager().addSystem<BungeeCordForceGeneratorSystem>();
	world.getSystemManager().addSystem<BuoyancySpawnSystem>();
	world.getSystemManager().addSystem<BuoyancyDensitySystem>();
	world.getSystemManager().addSystem<BuoyancyForceGeneratorSystem>();

	float time = glfwGetTime();
	float stepTime = glfwGetTime();
	float deltaTime = 0;
	float elapsedDeltaTime = 0;
	float logicDelta = 0;
	float debugDelta = 0;

	// game loop
	// -----------
	while (!glfwWindowShouldClose(world.data.renderUtil->window->glfwWindow))
	{
		float current = glfwGetTime();
		deltaTime = current - time;
		time = glfwGetTime();

		world.update();

		// Poll OpenGl events
		glfwPollEvents();

		world.data.renderUtil->ClearDisplay(world.data.renderUtil->window->glfwWindow);

		// Process Input
		world.getSystemManager().getSystem<InputEventSystem>().Update(deltaTime);

		// Game Logic Update
		world.getSystemManager().getSystem<FPSControlSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RotateSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<ParticleSpawnerSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<BungeeSpawnSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<BuoyancySpawnSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<BuoyancyDensitySystem>().Update(deltaTime);

		// Physics
		// Force Generators
		//world.getSystemManager().getSystem<GravityForceGeneratorSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<BungeeCordForceGeneratorSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<BuoyancyForceGeneratorSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<FixedSpringForceGeneratorSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<PairedSpringForceGeneratorSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<NBodyForceGeneratorSystem>().Update(deltaTime);

		// Force Accumulation
		world.getSystemManager().getSystem<ForceAccumulatorSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<ParticleSystem>().Update(deltaTime);

		// Rendering Update
		world.getSystemManager().getSystem<RenderingSystem>().Update(deltaTime);

		elapsedDeltaTime = glfwGetTime() - time;
		logicDelta = elapsedDeltaTime - world.data.renderUtil->GetRenderDelta();
		stepTime = glfwGetTime();

		// Debug
		if (DEBUG_LOG_LEVEL > 0)
		{
			world.data.renderUtil->RenderText("FPS : " + std::to_string((int)round(1.0f / deltaTime)), 1810.0f, 1060.0f, 0.5f, Color(0, 1, 1, 1));
		}
		if (DEBUG_LOG_LEVEL > 1)
		{
			int logic = (int)round(logicDelta * 100.0f / deltaTime);
			std::string logicString = logic < 10 ? " " + std::to_string(logic) : std::to_string(logic);
			int render = (int)round(world.data.renderUtil->GetRenderDelta() * 100.0f / deltaTime);
			std::string renderString = logic < 10 ? " " + std::to_string(render) : std::to_string(render);
			int debug = (int)round(debugDelta * 100.0f / deltaTime);
			std::string debugString = logic < 10 ? " " + std::to_string(debug) : std::to_string(debug);
			
			world.data.renderUtil->RenderText("Logic : " + logicString + "%" +
				//+ " | Physics : " + std::to_string((int)round(physicsDelta * 100.0f / deltaTime)) + "%" +
				+ " | Rendering : " + renderString + "%" +
				+ " | Debug : " + debugString + "%"
				, 1680.0f, 1040.0f, 0.25f, Color(0, 1, 1, 1));
		}
		if (DEBUG_LOG_LEVEL > 2)
		{
			world.data.renderUtil->RenderText("Draw Calls : " + std::to_string(world.data.renderUtil->GetDrawCalls())
				+ " | Verts : " + std::to_string(world.data.renderUtil->GetVerts())
				+ " | Tris : " + std::to_string(world.data.renderUtil->GetTris())
				+ " | Lines : " + std::to_string(world.data.renderUtil->GetLines())
				, 1610.0f, 1020.0f, 0.25f, Color(0, 1, 1, 1));
		}

		// Update debug delta
		debugDelta = glfwGetTime() - stepTime;
		stepTime = glfwGetTime();

		world.data.renderUtil->SwapBuffers(world.data.renderUtil->window->glfwWindow);

		// Show FPS in console
		//std::cout << "FPS : " << 1.0f / deltaTime << std::endl;
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void LoadAssets(ECSWorld& world)
{
	//world.data.renderUtil->LoadModel("Resources/Models/nanosuit/nanosuit.obj");
	//world.data.renderUtil->LoadModel("Resources/Models/Stan_Lee_obj/Stan Lee.obj")->SetTexture("Resources/Models/Stan_Lee_obj/Stan Lee_Diffuse.png");
	world.data.renderUtil->LoadModel("Resources/Models/ribbon-ball/model.obj")->SetTexture("Resources/Models/ribbon-ball/tex_u1_v1.jpg");
	/*Model* robot = world.data.renderUtil->LoadModel("Resources/Models/Robot/source/Zbot_Animation.fbx");
	robot->SetTexture("Resources/Models/Robot/textures/Ground_Base_Color_Transparency.png");
	robot->SetTexture("Resources/Models/Robot/textures/MAT_Body_Base_Color.png", 1);*/
	//world.data.renderUtil->LoadModel("Resources/Models/bagan-khayiminga-temple-interior/model.obj")->SetTexture("Resources/Models/bagan-khayiminga-temple-interior/tex_u1_v1.jpg");

	Model* classroom = world.data.renderUtil->LoadModel("Resources/Models/animeclassroom/anime school.obj");

	world.data.renderUtil->LoadShader("Shaders/vertexDefault.vs", "Shaders/fragmentDefault.fs");
}

void MakeABunchaSprings(ECSWorld& world)
{
	auto e = world.createEntity();
	e.addComponent<TransformComponent>(Vector3(-2.5f, -5, -3), Vector3(0.1f, 0.1f, 0.1f));
	e.addComponent<ParticleComponent>();
	// Add mesh
	e.addComponent<MeshComponent>(
		"Resources/Models/ribbon-ball/model.obj",
		"Shaders/vertexDefault.vs",
		"Shaders/fragmentDefault.fs");

	auto springEntinty = world.createEntity();
	springEntinty.addComponent<TransformComponent>(Vector3(-2.5f, 0, 3));
	springEntinty.addComponent<FixedSpringComponent>(8, 2, e);

	auto e2 = world.createEntity();
	e2.addComponent<TransformComponent>(Vector3(2.5f, -5, -1), Vector3(0.1f, 0.1f, 0.1f));
	e2.addComponent<ParticleComponent>();
	// Add mesh
	e2.addComponent<MeshComponent>(
		"Resources/Models/ribbon-ball/model.obj",
		"Shaders/vertexDefault.vs",
		"Shaders/fragmentDefault.fs");

	auto springEntinty2 = world.createEntity();
	springEntinty2.addComponent<TransformComponent>(Vector3(2.5f, 0, 1));
	springEntinty2.addComponent<FixedSpringComponent>(5, 5, e2);

	auto pairedSpring = world.createEntity();
	pairedSpring.addComponent<PairedSpringComponent>(100, 5.0f, e, e2);

	auto e3 = world.createEntity();
	e3.addComponent<TransformComponent>(Vector3(-7.5f, -7.5f, 1), Vector3(0.1f, 0.1f, 0.1f));
	e3.addComponent<ParticleComponent>();
	// Add mesh
	e3.addComponent<MeshComponent>(
		"Resources/Models/ribbon-ball/model.obj",
		"Shaders/vertexDefault.vs",
		"Shaders/fragmentDefault.fs");

	auto springEntinty3 = world.createEntity();
	springEntinty3.addComponent<TransformComponent>(Vector3(-7.5f, -10, -1));
	springEntinty3.addComponent<FixedSpringComponent>(7, 7, e3);

	auto e4 = world.createEntity();
	e4.addComponent<TransformComponent>(Vector3(7.5f, -7.5f, 3), Vector3(0.1f, 0.1f, 0.1f));
	e4.addComponent<ParticleComponent>();
	// Add mesh
	e4.addComponent<MeshComponent>(
		"Resources/Models/ribbon-ball/model.obj",
		"Shaders/vertexDefault.vs",
		"Shaders/fragmentDefault.fs");

	auto springEntinty4 = world.createEntity();
	springEntinty4.addComponent<TransformComponent>(Vector3(7.5f, -10, -3));
	springEntinty4.addComponent<FixedSpringComponent>(5, 0, e4);

	auto pairedSpring2 = world.createEntity();
	pairedSpring2.addComponent<PairedSpringComponent>(100, 5.2f, e, e3);

	auto pairedSpring3 = world.createEntity();
	pairedSpring3.addComponent<PairedSpringComponent>(100, 5.2f, e2, e4);

	auto pairedSpring4 = world.createEntity();
	pairedSpring4.addComponent<PairedSpringComponent>(100, 10.0f, e3, e4);
}

void MakeABunchaPlanets(ECSWorld& world) 
{
	auto e = world.createEntity();
	e.addComponent<TransformComponent>(Vector3(-2.5f, -3, -3));
	e.addComponent<ParticleComponent>(10, Vector3(1, 1, 1));
	e.addComponent<SphereComponent>(1);

	auto e1 = world.createEntity();
	e1.addComponent<TransformComponent>(Vector3(-5, -6, -2));
	e1.addComponent<ParticleComponent>(15, Vector3(1, -2, 1));
	e1.addComponent<SphereComponent>(1);

	auto e2 = world.createEntity();
	e2.addComponent<TransformComponent>(Vector3(2.5f, 10, -1));
	e2.addComponent<ParticleComponent>(5, Vector3(-1.2, -0.5, 0.1));
	e2.addComponent<SphereComponent>(1);

	auto e3 = world.createEntity();
	e3.addComponent<TransformComponent>(Vector3(-2, -7, -7));
	e3.addComponent<ParticleComponent>(20, Vector3(0, 0.3, -0.75));
	e3.addComponent<SphereComponent>(1);

	auto e4 = world.createEntity();
	e4.addComponent<TransformComponent>(Vector3(-0.5f, -4, 4));
	e4.addComponent<ParticleComponent>(30, Vector3(0.6, 0.1, -0.2));
	e4.addComponent<SphereComponent>(1);

	auto e5 = world.createEntity();
	e5.addComponent<TransformComponent>(Vector3(0.5f, 2, 1));
	e5.addComponent<ParticleComponent>(2, Vector3(0, 0, 0.3));
	e5.addComponent<SphereComponent>(1);

	auto e6 = world.createEntity();
	e6.addComponent<TransformComponent>(Vector3(1.5f, 1, 0));
	e6.addComponent<ParticleComponent>(12, Vector3(-0.4, 0.2, -0.1));
	e6.addComponent<SphereComponent>(1);

	auto e7 = world.createEntity();
	e7.addComponent<TransformComponent>(Vector3(-3.5f, 8, 2));
	e7.addComponent<ParticleComponent>(210, Vector3(0, -0, 0));
	e7.addComponent<SphereComponent>(1);

	auto e8 = world.createEntity();
	e8.addComponent<TransformComponent>(Vector3(2.5f, 0, -2));
	e8.addComponent<ParticleComponent>(8, Vector3(0.1, 0.4, 0));
	e8.addComponent<SphereComponent>(1);

	auto e9 = world.createEntity();
	e9.addComponent<TransformComponent>(Vector3(3, -1, -6));
	e9.addComponent<ParticleComponent>(6, Vector3(0.5, 0.2, 0.4));
	e9.addComponent<SphereComponent>(1);

	auto nBody = world.createEntity();
	nBody.addComponent<NBodyComponent>(e, e1, e2, e3, e4, e5, e6, e7, e8, e9);
}

void MakeBungeeChordSeed(ECSWorld& world)
{
	auto e = world.createEntity();
	e.addComponent<TransformComponent>(Vector3(-2.5f, -3, -3));
	e.addComponent<BungeeSpawnComponent>();
	e.addComponent<SphereComponent>(0.5f);
}

void MakeBucket(ECSWorld& world) {
	auto e = world.createEntity();
	e.addComponent<TransformComponent>(Vector3(-2.5f, -10, -7), Vector3(10.0f, 5.0f, 10.0f));
	e.addComponent<BoxComponent>();
	e.addComponent<BuoyancySpawnComponent>(10,0.1);
}
