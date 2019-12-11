#include "WaterSystem.h"
#include "TransformComponentV2.h"
#include "WaterComponent.h"
#include "SpawnTargetEvent.h"

namespace Reality
{
	WaterSystem::WaterSystem()
	{
		requireComponent<WaterComponent>();
		requireComponent<TransformComponentV2>();
	}

	void WaterSystem::Update(float deltaTime)
	{
		auto targetEvents = getWorld().getEventManager().getEvents<SpawnTargetEvent>();

		if (targetEvents.size() > 0)
		{
			Vector3 target = targetEvents[0].targetPos;


			for (auto e : getEntities())
			{
				auto& spawn = e.getComponent<WaterComponent>();
				auto& transform = e.getComponent<TransformComponentV2>();

				if (abs(target.z - transform.GetPosition().z) > 2050.0f)
				{
					transform.SetPosition(transform.GetPosition() + (target.z > transform.GetPosition().z ? 4050.0f : -4050.0f) * Vector3(0, 0, 1));
				}
					
				getWorld().data.renderUtil->DrawCube(transform.GetPosition() -(spawn.waterDepth/2) * Vector3(0,1,0), Vector3(400.0f, spawn.waterDepth, 100.0f), Vector3(0,0,0), Color::Cyan);
			}
		}



		for (auto e : getEntities())
		{
			
		}
	}
}
