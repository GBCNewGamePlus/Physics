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
					auto e = getWorld().createEntity();
					float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 4 - 2;
					float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 4 - 2;
					float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 4 - 2;
					e.addComponent<TransformComponent>(transformComponent.position + Vector3(rx, ry, rz));
					e.addComponent<ParticleComponent>();
					e.addComponent<BungeeSpawnComponent>();
					// without a mesh, this will drawn out as another sphere
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && !keyReleased) {
			keyReleased = true;
		}
	}
}
