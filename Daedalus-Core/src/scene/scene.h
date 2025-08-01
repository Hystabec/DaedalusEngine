#pragma once

#include "application/time/deltaTime.h"
#include "graphics/camera/editorCamera.h"
#include "application/uuid.h"

#include <entt.hpp>

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

		void updateRuntime(const application::DeltaTime& dt);
		void updateEditor(const application::DeltaTime& dt, graphics::EditorCamera& camera);

		Entity createEntity(const std::string& name = std::string());
		Entity createEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void destroyEntity(Entity entity);

		void onViewportResize(uint32_t width, uint32_t hegiht);

		Entity getPrimaryCameraEntity();
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		friend class Entity;
		friend class editor::SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}