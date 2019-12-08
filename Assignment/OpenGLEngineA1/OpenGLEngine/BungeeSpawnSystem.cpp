#include "BungeeSpawnSystem.h"

namespace Reality
{
	BungeeSpawnSystem::BungeeSpawnSystem()
	{
		requireComponent<BungeeSpawnComponent>();
		requireComponent<TransformComponent>();
	}
	void BungeeSpawnSystem::Update(float deltaTime)
	{
		GLFWwindow*  window = getWorld().data.renderUtil->window->glfwWindow;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && keyReleased)
		{
			keyReleased = false;
			for (auto e : getEntities())
			{
				auto &bungeeSpawnComponent = e.getComponent<BungeeSpawnComponent>();
				const auto &transformComponent = e.getComponent<TransformComponent>();
				// MoveS
				if (bungeeSpawnComponent.free) {
					bungeeSpawnComponent.free = false;
					// Spawns another Bungee from here
					auto subE = getWorld().createEntity();
					subE.addComponent<TransformComponent>(getWorld().data.renderUtil->camera.Position);
					subE.addComponent<ParticleComponent>();
					subE.addComponent<BungeeSpawnComponent>();
					subE.addComponent<BungeeCordComponent>(10, 2.5f, e, subE);
					subE.addComponent<SphereComponent>(1);
					// without a mesh, this will drawn out as another sphere
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && !keyReleased) {
			keyReleased = true;
		}
	}
}
