#include "SphereContactSystem.h"
#include "ParticleContactEvent.h"
#include "ParticleComponent.h"

namespace Reality
{
	SphereContactSystem::SphereContactSystem()
	{
		requireComponent<SphereComponent>();
		requireComponent<TransformComponent>();
		requireComponent<ParticleComponent>();
	}


	void SphereContactSystem::Update(float deltaTime)
	{
		for (auto e1 : getEntities()) {
			for (auto e2 : getEntities()) {

				auto& transform1 = e1.getComponent<TransformComponent>();
				auto& sphere1 = e1.getComponent<SphereComponent>();
				bool collided = false;
				// Collision with other spheres
				if (e1.getIndex() != e2.getIndex()) {
					auto& transform2 = e2.getComponent<TransformComponent>();
					auto& sphere2 = e2.getComponent<SphereComponent>();
					if (glm::length(transform1.position = transform2.position) < (sphere1.radius + sphere2.radius))
					{
						Vector3 normal = glm::normalize(transform1.position - transform2.position);
						getWorld().getEventManager().emitEvent<ParticleContactEvent>(e1,e2,0.9f,normal);
						collided = true;
					}
				}

				// Collision with walls
				// X-axis
				auto& particle = e1.getComponent<ParticleComponent>();
				if (abs(transform1.position.x) >= 5)
				{
					particle.velocity.x *= -1;
					collided = true;
				}
				if (abs(transform1.position.y) >= 5)
				{
					particle.velocity.y *= -1;
					collided = true;
				}
				if (abs(transform1.position.z) >= 5)
				{
					particle.velocity.z *= -1;
					collided = true;
				}
				Color col = collided ? Color(1,0,0,1):Color(0,1,0,1);
				getWorld().data.renderUtil->DrawSphere(transform1.position, sphere1.radius, col);
			}
		}
	}

}

