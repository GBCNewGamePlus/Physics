#include "RenderingSystem.h"
#include "Shader.h"
#include "Camera.h"

namespace Reality
{
	RenderingSystem::RenderingSystem()
	{
		requireComponent<TransformComponent>();
		//requireComponent<MeshComponent>();
	}

	void RenderingSystem::Update(float deltaTime)
	{
		for (auto e : getEntities()) 
		{
			const auto transform = e.getComponent<TransformComponent>();
			if (e.hasComponent<MeshComponent>()) {
				auto &mesh = e.getComponent<MeshComponent>();
				getWorld().data.renderUtil->GetShader(mesh.vertexShader, mesh.fragmentShader).use();

				// view/projection transformations
				float aspectRatio = (float)getWorld().data.renderUtil->window->width / (float)getWorld().data.renderUtil->window->height;
				glm::mat4 projection = glm::perspective(glm::radians(getWorld().data.renderUtil->camera.Zoom), aspectRatio, 0.1f, 100.0f);
				glm::mat4 view = getWorld().data.renderUtil->camera.GetViewMatrix();

				// world transformation
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, transform.position);
				model = glm::scale(model, transform.scale);
				model = glm::rotate(model, glm::radians(transform.eulerAngles.z), Vector3(0, 0, 1));
				model = glm::rotate(model, glm::radians(transform.eulerAngles.y), Vector3(0, 1, 0));
				model = glm::rotate(model, glm::radians(transform.eulerAngles.x), Vector3(1, 0, 0));

				getWorld().data.renderUtil->GetShader(mesh.vertexShader, mesh.fragmentShader).setMat4("projection", projection);
				getWorld().data.renderUtil->GetShader(mesh.vertexShader, mesh.fragmentShader).setMat4("view", view);
				getWorld().data.renderUtil->GetShader(mesh.vertexShader, mesh.fragmentShader).setMat4("model", model);

				// Draw
				getWorld().data.renderUtil->DrawModel(mesh.mesh, mesh.vertexShader, mesh.fragmentShader);
			}
			else if (e.hasComponent<BuoyancyComponent>()) {
				const auto &buoyancy = e.getComponent<BuoyancyComponent>();
				const auto &sphere = e.getComponent<SphereComponent>();
				getWorld().data.renderUtil->DrawBuoyancySphere(transform.position, sphere.radius, buoyancy.liquidHeight, 
					/* Yellowish for not below water*/  /* Blueish for below water */
					Color(1, 1, 0.88f, 0.5),            Color(0.529f, 0.808f, 0.922f, 0.5));
			}
			else if(e.hasComponent<SphereComponent>())
			{
				auto &sphere = e.getComponent<SphereComponent>();
				getWorld().data.renderUtil->DrawSphere(transform.position, sphere.radius, Color(0,1,0,1));
			}
			else if (e.hasComponent<BoxComponent>()) {
				auto &box = e.getComponent<BoxComponent>();
				getWorld().data.renderUtil->DrawCube(transform.position, transform.scale, Vector3(0,0,0), Color(0, 1, 0, 0.3f));
			}
		}
	}
}

