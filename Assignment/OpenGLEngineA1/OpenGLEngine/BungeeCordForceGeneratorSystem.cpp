#include "BungeeCordForceGeneratorSystem.h"

namespace Reality
{
	BungeeCordForceGeneratorSystem::BungeeCordForceGeneratorSystem()
	{
		requireComponent<BungeeCordComponent>();
	}


	void BungeeCordForceGeneratorSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto &bungeeCord = e.getComponent<BungeeCordComponent>();

			if (bungeeCord.entityA.hasComponent<TransformComponent>() &&
				bungeeCord.entityB.hasComponent<TransformComponent>())
			{
				//auto &particle = spring.entity.getComponent<ParticleComponent>();
				auto &entityTransformA = bungeeCord.entityA.getComponent<TransformComponent>();
				auto &entityTransformB = bungeeCord.entityB.getComponent<TransformComponent>();

				Vector3 relativePosition = entityTransformA.position - entityTransformB.position;
				float length = glm::length(relativePosition);
				if (length > bungeeCord.restLength)
				{
					// Acts only if the lenght is larger than the rest length
					float x = length - bungeeCord.restLength;
					Vector3 direction = glm::normalize(relativePosition);
					if (bungeeCord.entityA.hasComponent<ParticleComponent>())
					{
						bungeeCord.entityA.getComponent<ParticleComponent>().AddForce(-bungeeCord.bungeeCordConstant * x * direction);
					}

					if (bungeeCord.entityB.hasComponent<ParticleComponent>())
					{
						bungeeCord.entityB.getComponent<ParticleComponent>().AddForce(bungeeCord.bungeeCordConstant * x * direction);
					}

					float g = 1.0f / (1.0f + pow(abs(x), 0.5f));
					float r = (1 - g);
					Color color = Color(r, g, 0, 1);

					float deltaLength = length / 10.0f;
					for (int i = 0; i < 10; i++)
					{
						getWorld().data.renderUtil->DrawCube(
							entityTransformB.position + (float)i * deltaLength * direction,
							Vector3(0.1f, 0.1f, 0.1f) * min((bungeeCord.bungeeCordConstant / 10.0f), 5.0f), Vector3(0, 0, 0), color);
					}

					getWorld().data.renderUtil->DrawLine(
						entityTransformB.position, entityTransformB.position + length * direction, color);

				}
			}
		}
	}
}