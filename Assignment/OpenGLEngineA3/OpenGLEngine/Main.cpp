//#define STB_IMAGE_IMPLEMENTATION
#include "UpdateTransformMatricesSystem.h"
#include "RenderingSystem.h"
#include "RenderingSystemV2.h"
#include "InputEventSystem.h"
#include "RotateSystem.h"
#include "ParticleSystem.h"
#include "ParticleSpawnerSystem.h"
#include "GravityForceGeneratorSystem.h"
#include "FixedSpringForceGeneratorSystem.h"
#include "ForceAccumulatorSystem.h"
#include "PairedSpringForceGeneratorSystem.h"
#include "SphereContactGeneratorSystem.h"
#include "ParticleContactResolutionSystem.h"
#include "CableComponentSystem.h"
#include "RodSystem.h"
#include "ForceAndTorqueAccumulatorSystem.h"
#include "RigidBodySystem.h"
#include "FPSControlSystem.h"
#include "DynamicDirectionalLightSystem.h"
#include "DynamicPointLightSystem.h"
#include "DynamicSpotLightSystem.h"
#include "FlightSimulatorSystem.h"
#include "FollowCameraSystem.h"
#include "InfiniteSpawnSystem.h"
#include "InfiniteSpawnTargetSystem.h"
#include "AeroControlSystem.h"
#include "SetAerodynamicTensorSystem.h"
#include "AeroSystem.h"
#include "CameraLookSystem.h"
#include "LifeTimeSystem.h"

#include "WaterComponent.h"
#include "WaterSystem.h"
#include "BuoyancyComponent.h"
#include "BuoyancyDensitySystem.h"
#include "BuoyancyForceGeneratorSystem.h"

#include <string>
#include <stdlib.h>     
#include <time.h>  

#define DEBUG_LOG_LEVEL 3

using namespace Reality;

void LoadShaders(ECSWorld& world);
void LoadModels(ECSWorld& world);
void MakeFlight(ECSWorld& world);
void SetupLights(ECSWorld& world);
void RenderStats(ECSWorld& world, float deltaTime, float logicDelta, float debugDelta);

int main()
{
	ECSWorld world;

	// Init and Load
	world.data.InitRendering();
	
	world.data.renderUtil->camera.Position = Vector3(0, 15.0f, 100.0f);
	world.data.renderUtil->SetFOV(60);
	// Create entities

	// Make a player controller
	auto e = world.createEntity();
	e.addComponent<FPSControlComponent>();
	//MakeABunchaObjects(world);
	MakeFlight(world);
	SetupLights(world);

	//MakeFlight(world);

	// Create Systems
	world.getSystemManager().addSystem<UpdateTransformMatricesSystem>();
	world.getSystemManager().addSystem<RenderingSystem>();
	world.getSystemManager().addSystem<RenderingSystemV2>();
	world.getSystemManager().addSystem<InputEventSystem>();
	world.getSystemManager().addSystem<RotateSystem>();
	world.getSystemManager().addSystem<ParticleSystem>();
	world.getSystemManager().addSystem<ParticleSpawnerSystem>();
	world.getSystemManager().addSystem<GravityForceGeneratorSystem>();
	world.getSystemManager().addSystem<ForceAndTorqueAccumulatorSystem>();
	world.getSystemManager().addSystem<RigidBodySystem>();
	world.getSystemManager().addSystem<FlightSimulatorSystem>();
	world.getSystemManager().addSystem<FPSControlSystem>();
	world.getSystemManager().addSystem<FollowCameraSystem>();
	world.getSystemManager().addSystem<CameraLookSystem>();
	world.getSystemManager().addSystem<InfiniteSpawnSystem>();
	world.getSystemManager().addSystem<InfiniteSpawnTargetSystem>();
	world.getSystemManager().addSystem<AeroControlSystem>();
	world.getSystemManager().addSystem<SetAerodynamicTensorSystem>();
	world.getSystemManager().addSystem<AeroSystem>();
	world.getSystemManager().addSystem<LifeTimeSystem>();
	world.getSystemManager().addSystem<DynamicDirectionalLightSystem>();
	world.getSystemManager().addSystem<DynamicPointLightSystem>();
	world.getSystemManager().addSystem<DynamicSpotLightSystem>();

	world.getSystemManager().addSystem<WaterSystem>();
	world.getSystemManager().addSystem<BuoyancyDensitySystem>();
	world.getSystemManager().addSystem<BuoyancyForceGeneratorSystem>();

	float time = glfwGetTime();
	float stepTime = glfwGetTime();
	float deltaTime = 0;
	float elapsedDeltaTime = 0;
	float logicDelta = 0;
	float debugDelta = 0;

	LoadShaders(world);
	bool shadersLoaded = false;
	bool modelsLoadStarted = false;
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

		// Load
		if (!shadersLoaded)
		{
			shadersLoaded = world.data.assetLoader->ShadersLoaded();
		}
		if(shadersLoaded && !modelsLoadStarted)
		{
			LoadModels(world);
			modelsLoadStarted = true;
		}
		// Process Input
		world.getSystemManager().getSystem<InputEventSystem>().Update(deltaTime);

		// Game Logic Update
		world.getSystemManager().getSystem<FPSControlSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RotateSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<ParticleSpawnerSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<BuoyancyDensitySystem>().Update(deltaTime);

		//Flight Sim
		world.getSystemManager().getSystem<FlightSimulatorSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<FollowCameraSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<CameraLookSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<InfiniteSpawnTargetSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<InfiniteSpawnSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<AeroControlSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<SetAerodynamicTensorSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<LifeTimeSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<WaterSystem>().Update(deltaTime);

		// Update Transform
		world.getSystemManager().getSystem<UpdateTransformMatricesSystem>().Update(deltaTime);
		// Physics
		float fixedDeltaTime = 1 / 60.0f;
		world.getSystemManager().getSystem<AeroSystem>().Update(fixedDeltaTime);
		// Force Generators
		world.getSystemManager().getSystem<GravityForceGeneratorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<BuoyancyForceGeneratorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<ForceAndTorqueAccumulatorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<RigidBodySystem>().Update(fixedDeltaTime);

		// Rendering Update
		world.getSystemManager().getSystem<DynamicDirectionalLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<DynamicPointLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<DynamicSpotLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RenderingSystemV2>().Update(deltaTime);

		elapsedDeltaTime = glfwGetTime() - time;
		logicDelta = elapsedDeltaTime - world.data.renderUtil->GetRenderDelta();
		stepTime = glfwGetTime();

		RenderStats(world, deltaTime, logicDelta, debugDelta);
		// Update debug delta
		debugDelta = glfwGetTime() - stepTime;
		stepTime = glfwGetTime();
		world.data.renderUtil->SwapBuffers(world.data.renderUtil->window->glfwWindow);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void RenderStats(ECSWorld& world, float deltaTime, float logicDelta, float debugDelta) {
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
			+" | Rendering : " + renderString + "%" +
			+" | Debug : " + debugString + "%"
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
}

void LoadShaders(ECSWorld& world)
{
	world.data.assetLoader->StartShaderLoading({ {"Shaders/Lighting_Maps.vs", "Shaders/Lighting_Maps.fs"} });
	world.data.assetLoader->StartShaderLoading({ {"Shaders/vertexDefault.vs", "Shaders/FragmentConstant.fs"} });
	world.data.assetLoader->StartShaderLoading({ {"Shaders/VertexBuoyancy.vs", "Shaders/FragmentBuoyancy.fs"} });
}

void LoadModels(ECSWorld& world)
{
	world.data.assetLoader->StartModelLoading({
		ModelData("Resources/Models/kapal.fbx",
			{{"castle.png"}})
		});
}

void MakeFlight(ECSWorld& world)
{
	auto boat = world.createEntity();
	glm::vec3 rotationInRads = glm::vec3(glm::radians(-90.0f),
		glm::radians(180.0f), glm::radians(0.0f));
	Quaternion orientation = glm::quat(rotationInRads);
	boat.addComponent<TransformComponentV2>(Vector3(0, 0, 0), Vector3(5, 5, 5));
	// Add mesh
	boat.addComponent<ModelComponent>("Resources/Models/kapal.fbx", Vector3(0, 1.0f, 2.0f), Vector3(-90, 0, 0));
	boat.addComponent<RigidBodyComponent>(10.0f ,0.3f, 0.5f);
	boat.addComponent<FlighSimulatorComponent>();
	boat.addComponent<FollowCameraComponent>(Vector3(0.0f, 15.0f, 40.0f));
	boat.addComponent<CameraLookComponent>();
	boat.addComponent<InfiniteSpawnTargetComponent>();
	boat.addComponent<BuoyancyComponent>(Vector4(0.0f,0.0f,0.001f,0.0f), 0.0f,-10000.0f);

	//Rudder 
	std::vector<int> pR = { GLFW_KEY_A };
	std::vector<int> nR = { GLFW_KEY_D };
	auto R = world.createEntity();
	// Controls
	R.addComponent<AeroControlComponent>(pR, nR, 0.5);
	// Max/Min Tensors
	R.addComponent<AeroMinMaxComponent>
		( /* Min Tensor */
		  Mat3(0,      0, 0, 
			   0,      0, 0, 
			   0.002f, 0, 0),
		  /* Base Tensor */
		  Mat3(0, 0, 0, 
			   0, 0, 0, 
			   0, 0, 0),
		  /* Max Tensor */
		  Mat3(0,       0, 0, 
			   0,       0, 0, 
			   -0.002f, 0, 0));

	// Apply Tensors to the boat to control its position
	R.addComponent<AeroComponent>(boat, Mat3(1.0f), Vector3(0, 0, -200.0f));

	// Creates the "inifinte" channel between buildings
	int runway = 40;
	for (int i = -runway; i <= runway; i++)
	{
		auto water = world.createEntity();
		water.addComponent<WaterComponent>(50);
		water.addComponent<TransformComponentV2>(Vector3(0, 0, 50.0f * i));

		auto buildingR = world.createEntity();
		buildingR.addComponent<TransformComponentV2>(Vector3(200.0f, 0.0f, 50.0f * i));
		buildingR.addComponent<InfiniteSpawnComponent>(RANDOM_FLOAT(100.0f, 500.0f));

		auto buildingL = world.createEntity();
		buildingL.addComponent<TransformComponentV2>(Vector3(-200.0f, 0.0f, 50.0f * i));
		buildingL.addComponent<InfiniteSpawnComponent>(RANDOM_FLOAT(100.0f, 500.0f));

	}
}

void SetupLights(ECSWorld& world)
{
	auto l = world.createEntity();
	l.addComponent<TransformComponent>(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(90, 0, 0));
	l.addComponent<DynamicDirectionalLightComponent>(Color(1, 1, 1), Color::White, Color::Orange);

	// Lanterns
	auto pl1 = world.createEntity();
	pl1.addComponent<TransformComponent>(Vector3(22, 14, 48.5f));
	pl1.addComponent<DynamicPointLightComponent>(100000.0f, Color(1, 1, 1), Color(1.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f));
	pl1.addComponent<ParticleComponent>();
	auto hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(23, 15, 48.0f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl1);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22, 13.5f, 50.5f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl1);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(21, 12.5f, 47.5f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl1);

	auto pl2 = world.createEntity();
	pl2.addComponent<TransformComponent>(Vector3(-14.5f, 14, 49.0f));
	pl2.addComponent<DynamicPointLightComponent>(100000.0f, Color(1, 1, 1), Color(0.0f, 0.0f, 1.0f), Color(0.0f, 0.0f, 1.0f));
	pl2.addComponent<ParticleComponent>();
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f + 1, 14 - 1, 49.0f - 1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl2);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 0.5f, 14 + 1, 49.0f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl2);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f, 14 - 1, 49.0f + 1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl2);
	
	auto pl3 = world.createEntity();
	pl3.addComponent<TransformComponent>(Vector3(22, 14, -62.0f));
	pl3.addComponent<DynamicPointLightComponent>(100000.0f, Color(1, 1, 1), Color(0.0f, 1.0f, 0.0f), Color(0.0f, 1.0f, 0.0f));
	pl3.addComponent<ParticleComponent>();
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22 - 1, 14 - 1, -62.0f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl3);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22, 14 + 0.5f, -62.0f - 1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl3);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22 + 1, 14, -62.0f + 0.5f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl3);

	auto pl4 = world.createEntity();
	pl4.addComponent<TransformComponent>(Vector3(-14.5f, 14, -61.5f));
	pl4.addComponent<DynamicPointLightComponent>(10000.0f, Color(1, 1, 1), Color(1.0f, 0.55f, 0.0f), Color(1.0f, 0.55f, 0.0f));
	pl4.addComponent<ParticleComponent>();
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 1, 14, -61.5f -1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl4);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 0.25f, 14 - 0.5f, -61.5f + 1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl4);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f + 0.5f, 14+ 1, -61.5f + 1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl4);

	// Spears
	std::vector<Color> cols = { Color(1,0,0), Color(0,1,0), Color(0,0,1), Color(0.7f,0.55f,0) };
	for (int i = 1; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pl1 = world.createEntity();
			pl1.addComponent<TransformComponent>(Vector3((i % 2 == 0 ? 8 : -1), 85, 49.5f - 37 * j), Vector3(1, 1, 1), Vector3(180, 0, 0));
			pl1.addComponent<DynamicSpotLightComponent>(10.0f, 100, Color(0, 0, 0), cols[3 - j], cols[3 - j], 5);
			pl1.addComponent<RotateComponent>((i % 2 == 0 ? 1 : -1) * 100,100,100);
		}
	}
}