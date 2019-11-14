#include "SphereContactGeneratorSystem.h"
#include "ParticleContactComponent.h"
#include "..\Include\assimp\vector3.h"
#include "TriangleMath.h"
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

		for (auto triangleEntity : getEntities())
		{
			auto triangleComponent = triangleEntity.getComponent<TriangleComponent>();
			float distance = triangleComponent.PointDistanceToPlane(spherePosition);
			if (distance <= sphereComponent.radius) {
				float penetration = sphereComponent.radius- abs(distance);
				ECSEntity e = getWorld().createEntity();
				e.addComponent<ParticleContactComponent>
					(sphereEntity, e, 0.05f, triangleComponent.normal, penetration);
			}
		}
	}
}
