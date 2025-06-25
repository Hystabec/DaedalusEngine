#include "ddpch.h"
#include "scene.h"

#include "graphics/renderer/renderer2D.h"
#include "entity.h"

namespace daedalus::scene {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::update(const application::DeltaTime& dt)
	{
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			
			graphics::Renderer2D::drawQuad({ transform.Transform, sprite.Colour });
		}
	}

	Entity Scene::createEntity(const std::string& name)
	{
		Entity entity = { m_registry.create(), this };
		entity.addComponent<TransformComponent>(maths::mat4::identity());

		auto& tag = entity.addComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

}