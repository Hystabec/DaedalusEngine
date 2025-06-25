#pragma once

#include <entt.hpp>

#include "application/time/deltaTime.h"
#include "components.h"

namespace daedalus::scene {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void update(const application::DeltaTime& dt);

		Entity createEntity(const std::string& name = std::string());
	private:
		entt::registry m_registry;

		friend class Entity;
	};

}