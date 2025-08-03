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

struct b2WorldId;
struct b2BodyId;
//foward declare so that Box2D doesnt need to be included and linked in other projects

namespace daedalus::scene {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Shr_ptr<Scene> copy(Shr_ptr<Scene> src);

		Entity createEntity(const std::string& name = std::string());
		Entity createEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void destroyEntity(Entity entity);
		void duplicateEntity(Entity entity);

		void onRuntimeStart();
		void onRutimeStop();

		void updateRuntime(const application::DeltaTime& dt);
		void updateEditor(const application::DeltaTime& dt, graphics::EditorCamera& camera);

		void onViewportResize(uint32_t width, uint32_t hegiht);

		Entity getPrimaryCameraEntity();
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		// This being a unique ptr is a bit weird, consider changing later
		Uni_ptr<b2WorldId> m_physicsWorld = nullptr;
		std::unordered_map<UUID, b2BodyId> m_entityBox2DBodyMap;

		friend class Entity;
		friend class editor::SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}