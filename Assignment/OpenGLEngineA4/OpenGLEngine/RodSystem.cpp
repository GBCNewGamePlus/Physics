#include "RodSystem.h"
#include "TransformComponent.h"
#include "ParticleContactComponent.h"
#include "TransformComponentV2.h"

namespace Reality
{
	RodSystem::RodSystem()
	{
		requireComponent<RodComponent>();
	}

	void RodSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto& rod = e.getComponent<RodComponent>();
			float currentLength = glm::length(rod.entityA.getComponent<TransformComponentV2>().GetPosition() -
				rod.entityB.getComponent<TransformComponentV2>().GetPosition());

			///getWorld().data.renderUtil->DrawSphere(rod.entityA.getComponent<TransformComponentV2>().GetPosition(), 1, Color::Purple);
			//getWorld().data.renderUtil->DrawSphere(rod.entityB.getComponent<TransformComponentV2>().GetPosition(), 1, Color::Purple);

			if (currentLength == rod.length)
			{
				continue;
			}

			Vector3 normal = glm::normalize(rod.entityB.getComponent<TransformComponentV2>().GetPosition() -
				rod.entityA.getComponent<TransformComponentV2>().GetPosition());


			if (currentLength > rod.length)
			{
				auto contactEntity = getWorld().createEntity();
				contactEntity.addComponent<ParticleContactComponent>(
					rod.entityA,
					rod.entityB,
					0,
					normal,
					currentLength - rod.length);
				//getWorld().data.renderUtil->DrawLine(rod.entityA.getComponent<TransformComponentV2>().GetPosition(),
				//	rod.entityB.getComponent<TransformComponentV2>().GetPosition(),
				//	Color::Yellow);
			}
			else
			{
				auto contactEntity = getWorld().createEntity();
				contactEntity.addComponent<ParticleContactComponent>(
					rod.entityA,
					rod.entityB,
					0,
					-normal,
					rod.length - currentLength);
				//getWorld().data.renderUtil->DrawLine(rod.entityA.getComponent<TransformComponentV2>().GetPosition(),
				//	rod.entityB.getComponent<TransformComponentV2>().GetPosition(),
				//	Color::Yellow);
			}

			
		}
	}
}
