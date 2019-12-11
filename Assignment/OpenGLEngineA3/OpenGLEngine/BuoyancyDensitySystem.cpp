#include "BuoyancyDensitySystem.h"

namespace Reality
{
	BuoyancyDensitySystem::BuoyancyDensitySystem()
	{
		requireComponent<BuoyancyComponent>();
	}
	void BuoyancyDensitySystem::Update(float deltaTime)
	{
		GLFWwindow*  window = getWorld().data.renderUtil->window->glfwWindow;
		for (auto e : getEntities())
		{
			auto &buoyancy = e.getComponent<BuoyancyComponent>();
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && keyReleased)
			{
				keyUsed = GLFW_KEY_UP;
				keyReleased = false;
				buoyancy.liquidDensity += buoyancy.densityStep;
				getWorld().data.renderUtil->RenderText("Current Liquid Density is going Up",
					1540.0f, 1000.0f, 0.25f, Color(0, 1, 1, 1));
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && keyReleased)
			{
				keyUsed = GLFW_KEY_DOWN;
				keyReleased = false;
				auto &buoyancy = e.getComponent<BuoyancyComponent>();
				if (buoyancy.liquidDensity > buoyancy.densityStep)
				{
					buoyancy.liquidDensity -= buoyancy.densityStep;
				}
				getWorld().data.renderUtil->RenderText("Current Liquid Density is going down",
					1540.0f, 1000.0f, 0.25f, Color(0, 1, 1, 1));
			}
			if (keyUsed == GLFW_KEY_UP && glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && !keyReleased) {
				keyReleased = true;
			}
			if (keyUsed == GLFW_KEY_DOWN && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && !keyReleased) {
				keyReleased = true;
			}
			std::string densityString = std::to_string(buoyancy.liquidDensity);
			getWorld().data.renderUtil->RenderText("Current Liquid Density is " + densityString,
				1540.0f, 1000.0f, 0.25f, Color(0, 1, 1, 1));
		}
	}
}
