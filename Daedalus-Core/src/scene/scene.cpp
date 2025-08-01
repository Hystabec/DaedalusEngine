#include "ddpch.h"
#include "scene.h"

#include "graphics/renderer/renderer2D.h"
#include "entity.h"
#include "entityComponents/components.h"

namespace daedalus::scene {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::updateRuntime(const application::DeltaTime& dt)
	{
		// update scripts
		{
			m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.scriptBound)
					{
						DD_CORE_LOG_WARN("No Script bound to NativeScriptComponent");
						return;
					}

					// TO DO: Moveto Scene::onScenePlay
					if (!nsc.instance)
					{
						nsc.instance = nsc.instantiateScript();
						nsc.instance->m_entity = Entity{ entity, this };
						nsc.instance->onCreate();
					}

					nsc.instance->onUpdate(dt);
				});
		}


		// Render2D sprites
		graphics::Camera* mainCamera = nullptr;
		maths::Mat4 mainCameraTransform;
		{
			auto view = m_registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.primary)
				{
					mainCamera = &camera.camera;
					mainCameraTransform = transform.getTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			graphics::Renderer2D::begin(*mainCamera, mainCameraTransform);

			auto group = m_registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				graphics::Renderer2D::drawSprite( transform.getTransform(), sprite, (uint32_t)entity);

			}

			graphics::Renderer2D::end();
		}
	}

	void Scene::updateEditor(const application::DeltaTime& dt, graphics::EditorCamera& camera)
	{
		graphics::Renderer2D::begin(camera);

		auto group = m_registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			//graphics::Renderer2D::drawQuad({ transform.getTransform(), sprite.Colour }, (int)entity);
			graphics::Renderer2D::drawSprite(transform.getTransform(), sprite, (uint32_t)entity);
		}

		graphics::Renderer2D::end();
	}

	Entity Scene::createEntity(const std::string& name)
	{
		return createEntityWithUUID(UUID(), name);
	}

	Entity Scene::createEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_registry.create(), this };
		entity.addComponent<IDComponent>(uuid);
		entity.addComponent<TransformComponent>();

		auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::destroyEntity(Entity entity)
	{
		m_registry.destroy(entity);
	}

	void Scene::onViewportResize(uint32_t width, uint32_t hegiht)
	{
		m_viewportWidth = width;
		m_viewportHeight = hegiht;

		// resize non-fixed aspect ratio cameras
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComp = view.get<CameraComponent>(entity);
			if (!cameraComp.fixedAspectRatio)
				cameraComp.camera.setViewportSize(width, hegiht);
		}
	}

	Entity Scene::getPrimaryCameraEntity()
	{
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& cc = view.get<CameraComponent>(entity);
			if (cc.primary)
				return Entity{ entity, this };
		}

		return {};
	}

	template<typename T>
	void scene::Scene::onComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void scene::Scene::onComponentAdded<scene::IDComponent>(Entity entity, scene::IDComponent& component)
	{
	}

	template<>
	void scene::Scene::onComponentAdded<scene::TagComponent>(Entity entity, scene::TagComponent& component)
	{
	}

	template<>
	void scene::Scene::onComponentAdded<scene::TransformComponent>(Entity entity, scene::TransformComponent& component)
	{
	}

	template<>
	void scene::Scene::onComponentAdded<scene::CameraComponent>(Entity entity, scene::CameraComponent& component)
	{
		component.camera.setViewportSize(m_viewportWidth, m_viewportHeight);
	}

	template<>
	void scene::Scene::onComponentAdded<scene::SpriteRendererComponent>(Entity entity, scene::SpriteRendererComponent& component)
	{
	}

	template<>
	void scene::Scene::onComponentAdded<scene::NativeScriptComponent>(Entity entity, scene::NativeScriptComponent& component)
	{
	}

}