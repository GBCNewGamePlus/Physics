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
#include "RigidbodyGravityForceGeneratorSystem.h"
#include "ContactGenerationSystem.h"
#include "ContactResolutionSystem.h"
#include "SphereColliderSystem.h"
#include "BoxColliderSystem.h"
#include "MoveInBoundsSystem.h"
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

/* Ragdoll exclusive */
#include "JointComponent.h"
#include "JointSystem.h"

#include <string>
#include <stdlib.h>     
#include <time.h>      

#define DEBUG_LOG_LEVEL 3

using namespace Reality;

void LoadShaders(ECSWorld& world);
void LoadModels(ECSWorld& world);
void BuildRagdoll(ECSWorld& world);
void BuildFloor(ECSWorld& world);
void SetupLights(ECSWorld& world);

int main()
{
	ECSWorld world;

	// Init and Load
	world.data.InitRendering();
	//LoadAssets(world);
	
	world.data.renderUtil->camera.Position = Vector3(0, 15.0f, 100.0f);
	world.data.renderUtil->SetFOV(60);
	// Create entities

	// Make a player controller
	auto e = world.createEntity();
	e.addComponent<FPSControlComponent>();

	SetupLights(world);
	BuildRagdoll(world);
	BuildFloor(world);

	// Create Systems
	world.getSystemManager().addSystem<RenderingSystem>();
	world.getSystemManager().addSystem<RenderingSystemV2>();
	world.getSystemManager().addSystem<InputEventSystem>();
	world.getSystemManager().addSystem<RotateSystem>();
	world.getSystemManager().addSystem<ParticleSystem>();
	world.getSystemManager().addSystem<ParticleSpawnerSystem>();
	world.getSystemManager().addSystem<GravityForceGeneratorSystem>();
	world.getSystemManager().addSystem<ParticleContactResolutionSystem>();
	world.getSystemManager().addSystem<ForceAccumulatorSystem>();
	world.getSystemManager().addSystem<ForceAndTorqueAccumulatorSystem>();
	world.getSystemManager().addSystem<FPSControlSystem>();
	world.getSystemManager().addSystem<CameraLookSystem>();
	world.getSystemManager().addSystem<AeroControlSystem>();
	world.getSystemManager().addSystem<SetAerodynamicTensorSystem>();
	world.getSystemManager().addSystem<AeroSystem>();
	world.getSystemManager().addSystem<DynamicDirectionalLightSystem>();
	world.getSystemManager().addSystem<DynamicPointLightSystem>();
	world.getSystemManager().addSystem<DynamicSpotLightSystem>();

	// Rigidbody Physics
	rp3d::CollisionWorld rp3dWorld;
	world.getSystemManager().addSystem<RigidBodySystem>(rp3dWorld);
	world.getSystemManager().addSystem<ContactGenerationSystem>(rp3dWorld);
	world.getSystemManager().addSystem<ContactResolutionSystem>(rp3dWorld);
	world.getSystemManager().addSystem<SphereColliderSystem>(rp3dWorld);
	world.getSystemManager().addSystem<BoxColliderSystem>(rp3dWorld);
	world.getSystemManager().addSystem<RigidbodyGravityForceGeneratorSystem>();

	world.getSystemManager().addSystem<JointSystem>();

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
		deltaTime = 1 / 60.0f;
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
		// Update View
		world.data.renderUtil->UpdateViewMatrix();
		// Process Input
		world.getSystemManager().getSystem<InputEventSystem>().Update(deltaTime);

		// Game Logic Update
		world.getSystemManager().getSystem<FPSControlSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RotateSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<ParticleSpawnerSystem>().Update(deltaTime);

		// Update Transform
		// Physics
		float fixedDeltaTime = 1 / 60.0f;
		//float fixedDeltaTime = 1 / 60.0f;
		// Particle Force Generators
		world.getSystemManager().getSystem<JointSystem>().Update(fixedDeltaTime);

		// Rigiidbody Force Generators and collisions
		world.getSystemManager().getSystem<RigidbodyGravityForceGeneratorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<ForceAndTorqueAccumulatorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<RigidBodySystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<SphereColliderSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<BoxColliderSystem>().Update(fixedDeltaTime);
		// Physics Solvers
		world.getSystemManager().getSystem<ParticleContactResolutionSystem>().Update(fixedDeltaTime);

		world.getSystemManager().getSystem<ContactGenerationSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<ContactResolutionSystem>().Update(fixedDeltaTime);

		// Rendering Update
		world.getSystemManager().getSystem<DynamicDirectionalLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<DynamicPointLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<DynamicSpotLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RenderingSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RenderingSystemV2>().Update(deltaTime);

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

void LoadShaders(ECSWorld& world)
{
	world.data.assetLoader->StartShaderLoading({ {"Shaders/Lighting_Maps.vs", "Shaders/Lighting_Maps.fs"} });
}
void LoadModels(ECSWorld& world)
{
	world.data.assetLoader->StartModelLoading({
		ModelData("Resources/Models/supermarine-spitfire/spitfire.fbx",
			{{"spitfire_d.png"}})
		});
}

enum BodyPart {
	LeftLeg,
	RightLeg,
	LeftThigh,
	RightThigh,
	Hip,
	Abs,
	Chest,
	LeftArm,
	RightArm,
	LeftForearm,
	RightForearm,
	Head,
	Nothing = -1
};

void BuildRagdoll(ECSWorld& world)
{
	// Bodyparts -> from the perspective of who looks at them
	Vector3 bodyPartPosition[12] = {
		Vector3 ( 0.7f,  1.7f, 1), /* Left Leg      */
		Vector3 ( 4.1f,  1.7f, 1), /* Right Leg     */
		Vector3 ( 0.7f,  5.4f, 1), /* Left Thigh    */
		Vector3 ( 4.1f,  5.4f, 1), /* Right Thigh   */
		Vector3 ( 2.4f,  8.6f, 1), /* Hip           */
		Vector3 ( 2.4f, 10.3f, 1), /* Abs           */
		Vector3 ( 2.4f, 12.5f, 1), /* Chest         */
		Vector3 (-1.5f, 13,    1), /* Left Arm      */
		Vector3 ( 6.1f, 13,    1), /* Right Arm     */
		Vector3 (-4.2f, 13,    1), /* Left Forearm  */
		Vector3 ( 8.8f, 13,    1), /* Right Forearm */
		Vector3 ( 2.4f, 15.2f, 1)  /* Head          */};
	Vector3 bodyPartScale[12] = { 
		Vector3 (1,3,1) /* Left Leg      */,
		Vector3 (1,3,1) /* Right Leg     */,
		Vector3 (1,4,1) /* Left Thigh    */,
		Vector3 (1,4,1) /* Right Thigh   */,
		Vector3 (4,2,1) /* Hip           */,
		Vector3 (2,1,1) /* Abs           */,
		Vector3 (4,2,1) /* Chest         */,
		Vector3 (3,1,1) /* Left Arm      */,
		Vector3 (3,1,1) /* Right Arm     */,
		Vector3 (2,1,1) /* Left Forearm  */,
		Vector3 (2,1,1) /* Right Forearm */,
		Vector3 (2,2,1) /* Head          */ };
	
	int limbToLimb[12][4] = {
		/* -1 means no connection */
		/* Left Leg      */ {LeftThigh,   Nothing,      Nothing, Nothing  },
		/* Right Leg     */ {RightThigh,  Nothing,      Nothing, Nothing  },
		/* Left Thigh    */ {LeftLeg,     Hip,          Nothing, Nothing  },
		/* Right Thigh   */ {RightLeg,    Hip,          Nothing, Nothing  },
		/* Hip           */ {LeftThigh,   RightThigh,   Abs,     Nothing  },
		/* Abs           */ {Hip,         Chest,        Nothing, Nothing  },
		/* Chest         */ {Abs,         LeftArm,      RightArm,Head     },
		/* Left Arm      */ {LeftForearm, Chest,        Nothing, Nothing  },
		/* Right Arm     */ {Chest,       RightForearm, Nothing, Nothing  },
		/* Left Forearm  */ {LeftArm,     Nothing,      Nothing, Nothing  },
		/* Right Forearm */ {RightArm,    Nothing,      Nothing, Nothing  },
		/* Head          */ {Chest,       Nothing,      Nothing, Nothing  }
	};

	Vector3 sockets[12][4] = { 
		// Local position of all sockets to get the bones through and connect the limbs
		// (0,0,0) is an invalid socket
		/* Left Foot     */ {Vector3(0.0f,1.5f,0.0f), Vector3(0,0,0),     Vector3(0,0,0),    Vector3(0,0,0)},
		/* Right Foot    */ {Vector3(0.0f,1.5f,0.0f), Vector3(0,0,0),     Vector3(0,0,0),    Vector3(0,0,0)},
		/* Left Ankle    */ {Vector3(0,-2,0),         Vector3(0,2,0),     Vector3(0,0,0),    Vector3(0,0,0)},
		/* Right Ankle   */ {Vector3(0,-2,0),         Vector3(0,2,0),     Vector3(0,0,0),    Vector3(0,0,0)},
		/* Hip           */ {Vector3(-1,-1,0),        Vector3(1,-1,0),    Vector3(0,1,0),    Vector3(0,0,0)},
		/* Abs           */ {Vector3(0,-0.5f,0),      Vector3(0,0.5f,0),  Vector3(0,0,0),    Vector3(0,0,0)},
		/* Chest         */ {Vector3(0,-1,0),         Vector3(-2,0.5f,0), Vector3(2,0.5f,0), Vector3(0,1,0)},
		/* Left Arm      */ {Vector3(-1.5f,0,0),      Vector3(1.5f,0,0),  Vector3(0,0,0),    Vector3(0,0,0)},
		/* Right Arm     */ {Vector3(-1.5f,0,0),      Vector3(1.5f,0,0),  Vector3(0,0,0),    Vector3(0,0,0)},
		/* Left Forearm  */ {Vector3(1.0f,0,0),       Vector3(0,0,0),     Vector3(0,0,0),    Vector3(0,0,0)},
		/* Right Forearm */ {Vector3(-1.0f,0,0),      Vector3(0,0,0),     Vector3(0,0,0),    Vector3(0,0,0)},
		/* Head          */ {Vector3(0,-1.0f,0),      Vector3(0,0,0),     Vector3(0,0,0),    Vector3(0,0,0)},
	};
	ECSEntity limbList[12];
	ECSEntity jointList[11];
	int joints = 0;
	for (int limbIterator = LeftLeg; limbIterator <= Head; limbIterator++)
	{
		auto limb = world.createEntity();
		limb.addComponent<TransformComponentV2>(bodyPartPosition[limbIterator]);
		limb.addComponent<RigidBodyComponent>();
		auto limbCollider = world.createEntity();
		limbCollider.addComponent<BoxColliderComponent>(limb, bodyPartScale[limbIterator]);
		limbList[limbIterator] = limb;
		// Creating joints without cannabis
		for (int jointIterator = 0; jointIterator < 4; jointIterator++) {
			int* limbJoints = limbToLimb[limbIterator];
			if (limbJoints[jointIterator] > Nothing
				&& limbJoints[jointIterator] < limbIterator)
			{
				// We can make a joint!!!
				auto limbSocket = sockets[limbIterator][jointIterator];
				int theOtherLimbIdx = limbJoints[jointIterator];
				auto theOtherLimb = limbList[theOtherLimbIdx];
				int* otherLimbJoints = limbToLimb[theOtherLimbIdx];
				int otherJointIterator = 0;
				for (;otherJointIterator < 4; otherJointIterator++) {
					if (otherLimbJoints[otherJointIterator] == limbIterator) {
						break;
					}
				}
				auto theOtherLimbSocket = sockets[theOtherLimbIdx][otherJointIterator];

				auto limbSocketW = limb.getComponent<TransformComponentV2>().LocalToWorldDirection(limbSocket);
				auto theOtherLimbSocketW = theOtherLimb.getComponent<TransformComponentV2>().LocalToWorldDirection(theOtherLimbSocket);
				float initialDistance = glm::length(limbSocketW- theOtherLimbSocketW);
				auto joint = world.createEntity();
				joint.addComponent<JointComponent>(
					limb,               // The current Limb
					limbSocket,         // The current Limb Socket
					theOtherLimb,		// The referenced limb
					theOtherLimbSocket,	// The refedenced limb socket of choice
					initialDistance,
					0.001f, // Cut some slack
					true);
				jointList[joints] = joint;
				joints++;
			}
		}
	}
	/*
	jointList[0].getComponent<JointComponent>().show = true;
	jointList[1].getComponent<JointComponent>().show = true;
	jointList[2].getComponent<JointComponent>().show = true;
	jointList[3].getComponent<JointComponent>().show = true;
	jointList[4].getComponent<JointComponent>().show = true;
	jointList[5].getComponent<JointComponent>().show = true;
	jointList[6].getComponent<JointComponent>().show = true;
	jointList[7].getComponent<JointComponent>().show = true;
	jointList[8].getComponent<JointComponent>().show = true;
	jointList[9].getComponent<JointComponent>().show = true;
	jointList[10].getComponent<JointComponent>().show = true;*/
}

void BuildFloor(ECSWorld& world)
{
	// Floor 1
	auto floor1 = world.createEntity();
	floor1.addComponent<TransformComponentV2>(Vector3(0, -50, 0), Vector3(1, 1, 1), Vector3(0, 0, 0));
	floor1.addComponent<RigidBodyComponent>(100000.0f, 0.4f, 0.3f, Vector3(0, 0, 0), Vector3(0, 0, 0), 0);
	auto floorCol1 = world.createEntity();
	floorCol1.addComponent<BoxColliderComponent>(floor1, Vector3(1000, 10, 1000));
}

void SetupLights(ECSWorld& world)
{
	auto l = world.createEntity();
	l.addComponent<TransformComponent>(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(90, 0, 0));
	l.addComponent<DynamicDirectionalLightComponent>(Color(0.00, 0.0, 0), Color::White, Color::Orange);

	// Lanterns
	auto pl1 = world.createEntity();
	pl1.addComponent<TransformComponent>(Vector3(22, 14, 48.5f));
	pl1.addComponent<DynamicPointLightComponent>(100.0f, Color(0.1, 0, 0), Color(1.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f));
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
	pl2.addComponent<DynamicPointLightComponent>(100.0f, Color(0, 0, 0.1f), Color(0.0f, 0.0f, 1.0f), Color(0.0f, 0.0f, 1.0f));
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
	pl3.addComponent<DynamicPointLightComponent>(100.0f, Color(0, 0.1f, 0), Color(0.0f, 1.0f, 0.0f), Color(0.0f, 1.0f, 0.0f));
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
	pl4.addComponent<DynamicPointLightComponent>(100.0f, Color(0.1, 0.05, 0), Color(1.0f, 0.55f, 0.0f), Color(1.0f, 0.55f, 0.0f));
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