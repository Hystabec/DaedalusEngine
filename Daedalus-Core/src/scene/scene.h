#pragma once

#include "asset/asset.h"
#include "application/time/deltaTime.h"
#include "graphics/camera/editorCamera.h"
#include "application/uuid.h"
#include "physicsScene2D.h"

#include <entt.hpp>

namespace daedalus::editor
{
	class SceneHierarchyPanel; 
	// foward declared for use in editor
}

namespace daedalus::scene {

	class Entity;

	class Scene : public Asset
	{
	public:
		Scene();
		~Scene() override;

		static AssetType getStaticType() { return AssetType::Scene; }
		virtual AssetType getType() const { return getStaticType(); }

		static IntrusivePtr<Scene> copy(IntrusivePtr<Scene> src);

		Entity createEntity(const std::string& name = std::string());
		Entity createEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void destroyEntity(Entity entity);
		Entity duplicateEntity(Entity entity);

		// prefer finding entity using UUID 
		Entity findEntityByName(std::string_view name);
		Entity getEntityByUUID(UUID uuid);

		void onRuntimeStart();
		void onSimulateStart();
		void onRutimeStop();
		void onSimulateStop();

		void updateRuntime(const application::DeltaTime& dt);
		void updateSimulation(const application::DeltaTime& dt, graphics::EditorCamera& camera);
		void updateEditor(const application::DeltaTime& dt, graphics::EditorCamera& camera);

		void step(int frames = 1);

		PhysicsScene2D& getPhysicsScene2D() { return m_physicsScene; }

		void onViewportResize(uint32_t width, uint32_t hegiht);

		Entity getPrimaryCameraEntity();

		bool isRunning() const { return m_isRunning; }

		void setPaused(bool paused) { m_isPaused = paused; }
		bool isPaused() const { return m_isPaused; }

		template<typename... components>
		auto getAllEntitiesWith()
		{
			return m_registry.view<components...>();
		}
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);

		void physics2DStart();
		void physics2DStop();

		void scriptingStart();
		void scriptingStop();

		void renderSceneEditor(graphics::EditorCamera& camera);
	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		std::unordered_map<UUID, entt::entity> m_entityMap;

		bool m_isRunning = false;
		bool m_isPaused = false;
		int m_stepFrames = 0;

		PhysicsScene2D m_physicsScene;

		friend class Entity;
		friend class editor::SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}