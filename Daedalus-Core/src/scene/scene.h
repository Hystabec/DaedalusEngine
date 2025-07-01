#pragma once

#include <entt.hpp>

#include "application/time/deltaTime.h"

namespace daedalus::editor
{
	class SceneHierarchyPanel; 
	// foward declared for use in editor
}

namespace daedalus::scene {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void update(const application::DeltaTime& dt);

		Entity createEntity(const std::string& name = std::string());
		void destroyEntity(Entity entity);

		void onViewportResize(uint32_t width, uint32_t hegiht);
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		friend class Entity;
		friend class editor::SceneHierarchyPanel;
	};

}