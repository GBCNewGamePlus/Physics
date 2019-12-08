#pragma once
#include "Mix/World.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Reality
{
	// MIX ECS wrappers
	using ECSWorld = Mix::World;
	using ECSEntity = Mix::Entity;
	using ECSSystem = Mix::System;
	template<class T> using ECSEvent = Mix::Event<T>;
	using ECSEntityManager = Mix::EntityManager;
	using ECSSystemManager = Mix::SystemManager;
	using ECSEventManager = Mix::EventManager;

	// GLM Wrappers
	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Mat3 = glm::mat3;
	using Mat4 = glm::mat4;
	using Mat3x3 = glm::mat3x3;
	using Mat3x4 = glm::mat3x4;
	using Mat4x4 = glm::mat4x4;
}