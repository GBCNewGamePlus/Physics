#include "FlightSimulatorSystem.h"
#include "LifeTimeComponent.h"

namespace Reality
{
	FlightSimulatorSystem::FlightSimulatorSystem()
	{
		requireComponent<FlighSimulatorComponent>();
		requireComponent<RigidBodyComponent>();
		requireComponent<TransformComponentV2>();
	}

	void FlightSimulatorSystem::Update(float deltaTime)
	{
		timer += deltaTime;
		for (auto e : getEntities())
		{
			auto& rigidbody = e.getComponent<RigidBodyComponent>();
			auto& transform = e.getComponent<TransformComponentV2>();
			auto& flight = e.getComponent<FlighSimulatorComponent>();

			rigidbody.AddForce(transform.LocalToWorldDirection(flight.propulsion));

			// smoke
			if (timer > 0.1f)
			{
				auto e = getWorld().createEntity();
				e.addComponent<TransformComponentV2>(transform.GetPosition() - transform.Forward() * 15.0f);
				e.addComponent<LifeTimeComponent>(7.0f);
				timer = 0;
			}
		}
		float width = getWorld().data.renderUtil->window->width;
		float height = getWorld().data.renderUtil->window->height;
		getWorld().data.renderUtil->RenderText("A , D - Left/Right Controls", width / 2 - 100.0f, 0.2f * height, 0.4f, Color::Black);

	}
}
