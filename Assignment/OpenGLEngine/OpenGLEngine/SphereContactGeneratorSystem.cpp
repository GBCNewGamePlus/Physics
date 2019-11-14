#include "SphereContactGeneratorSystem.h"
#include "ParticleContactComponent.h"
#include "..\Include\assimp\vector3.h"
#include "TriangleMath.h"
#include "TriangleContactComponent.h"
#include "SphereSpawnSystem.h"


namespace Reality
{
	SphereContactGeneratorSystem::SphereContactGeneratorSystem()
	{
		requireComponent<TriangleComponent>();
	}
	void SphereContactGeneratorSystem::Update(float deltaTime)
	{

		auto sphereEntity = getWorld().getSystemManager().getSystem<SphereSpawnSystem>().UniqueSphere;
		if (!sphereEntity.getIndex())
			return;
		auto spherePosition = sphereEntity.getComponent<TransformComponent>().position;
		auto sphereComponent = sphereEntity.getComponent<SphereComponent>();
		ECSEntity chosenOne;
		bool collision = false;
		for (auto triangleEntity : getEntities())
		{
			auto triangleComponent = triangleEntity.getComponent<TriangleComponent>();
			float distance = triangleComponent.PointDistanceToPlane(spherePosition);
			if (distance <= sphereComponent.radius) {
				float penetration = sphereComponent.radius - abs(distance);
				ECSEntity e = getWorld().createEntity();
				e.addComponent<TriangleContactComponent>
					(sphereEntity, triangleComponent.entityA, triangleComponent.entityB, triangleComponent.entityC,
						1.0f, triangleComponent.normal, penetration);
				/*
				collision = true;
				if (distance < minDistance) {
					chosenOne = triangleEntity;
					minDistance = distance;
				}*/
			}
		}
	}
}
