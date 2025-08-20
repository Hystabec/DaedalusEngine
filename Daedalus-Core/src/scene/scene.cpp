#include "ddpch.h"
#include "scene.h"

#include "graphics/renderer/renderer2D.h"
#include "entity.h"
#include "entityComponents/components.h"
#include "scripting/scriptEngine.h"

#include <box2d/box2d.h>

namespace daedalus::scene {

	static b2BodyType rigid_body_type_to_box2D_body_type(scene::Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case daedalus::scene::Rigidbody2DComponent::BodyType::Static:
			return b2BodyType::b2_staticBody;
		case daedalus::scene::Rigidbody2DComponent::BodyType::Dynamic:
			return b2BodyType::b2_dynamicBody;
		case daedalus::scene::Rigidbody2DComponent::BodyType::Kinematic:
			return b2BodyType::b2_kinematicBody;
		}

		DD_CORE_ASSERT(false, "Unknown body type");
		return b2BodyType::b2_staticBody;
	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		physics2DStop();
	}

	template<typename... Component>
	static void copy_component(entt::registry& dst, const entt::registry& src, const std::unordered_map <UUID, entt::entity>& enttMap)
	{
		// an inline lambda that immediatly gets exapanded for each component passed to the function
		([&]()
			{
				auto view = src.view<Component>();
				for (auto& e : view)
				{
					UUID uuid = src.get<IDComponent>(e).ID;
					entt::entity eID = enttMap.at(uuid);
					DD_CORE_ASSERT(enttMap.find(uuid) != enttMap.end())

						auto& component = src.get<Component>(e);
					dst.emplace_or_replace<Component>(eID, component);
				}
			}(), ...);
	}

	template<typename... Component>
	static void copy_component(ComponentGroup<Component...>, entt::registry& dst, const entt::registry& src, const std::unordered_map <UUID, entt::entity>& enttMap)
	{
		copy_component<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void copy_component_if_exists(Entity dst, Entity src)
	{
		// an inline lambda that immediatly gets exapanded for each component passed to the function
		([&]()
			{
				if (src.hasComponent<Component>())
					dst.addOrRepalaceComponent<Component>(src.getComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void copy_component_if_exists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		copy_component_if_exists<Component...>(dst, src);
	}

	Shr_ptr<Scene> Scene::copy(Shr_ptr<Scene> src)
	{
		Shr_ptr<Scene> dest = create_shr_ptr<Scene>();

		dest->m_viewportWidth = src->m_viewportWidth;
		dest->m_viewportHeight = src->m_viewportHeight;

		auto& srcSceneReg = src->m_registry;
		auto& destSceneReg = dest->m_registry;

		std::unordered_map<UUID, entt::entity> enttMap;

		auto idView = srcSceneReg.view<IDComponent>();
		for (auto& e : idView)
		{	
			auto& idComp = srcSceneReg.get<IDComponent>(e);
			UUID uuid = idComp.ID;
			const auto& name = idComp.name;
			enttMap[uuid] = dest->createEntityWithUUID(uuid, name);;
		}

		// dont need to do ID and tag as they have already been done
		copy_component(AllComponents{}, destSceneReg, srcSceneReg, enttMap);

		return dest;
	}

	Entity Scene::createEntity(const std::string& name)
	{
		return createEntityWithUUID(UUID(), name);
	}

	Entity Scene::createEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_registry.create(), this };
		auto& idComp = entity.addComponent<IDComponent>(uuid, name.empty() ? "Entity" : name);
		entity.addComponent<TransformComponent>();
		entity.addComponent<TagComponent>();

		m_entityMap[uuid] = entity;

		return entity;
	}

	void Scene::destroyEntity(Entity entity)
	{
		m_registry.destroy(entity);
		m_entityMap.erase(entity.getUUID());
	}

	void Scene::duplicateEntity(Entity entity)
	{
		Entity newEntity = createEntity(entity.getName());
		copy_component_if_exists(AllComponents{}, newEntity, entity);
	}

	Entity Scene::getEntityByUUID(UUID uuid)
	{
		if (m_entityMap.find(uuid) != m_entityMap.end())
			return { m_entityMap.at(uuid), this };

		// NOTE: consider changing to assert
		DD_CORE_LOG_ERROR("Could not find entity by UUID: '{}'", uuid);
		return {};
	}

	void Scene::onRuntimeStart()
	{
		physics2DStart();
		scriptingStart();
	}

	void Scene::onSimulateStart()
	{
		physics2DStart();
	}

	void Scene::onRutimeStop()
	{
		physics2DStop();
		scriptingStop();
	}

	void Scene::onSimulateStop()
	{
		physics2DStop();
	}

	void Scene::updateRuntime(const application::DeltaTime& dt)
	{
		// update scripts
		{
			auto view = m_registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				scripting::ScriptEngine::updateEntityInstance(entity, dt);
			}

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

		// find main camera
		graphics::Camera* mainCamera = nullptr;
		maths::Mat4 mainCameraTransform;
		{
			auto view = m_registry.view<TransformComponent, CameraComponent>();
			view.each([&](const TransformComponent& transform, CameraComponent& camera)
				{
					if (camera.primary)
					{
						mainCamera = &camera.camera;
						mainCameraTransform = transform.getTransform();
					}
				});
		}

		// Physics
		{
			const int32_t subStepCount = 4;
			b2World_Step(*m_physicsWorld, dt, subStepCount);

			auto view = m_registry.view<Rigidbody2DComponent>();
			
			for(auto e : view)
			{
				Entity enity = { e, this };
				auto& transform = enity.getComponent<TransformComponent>();
				auto& rb2d = enity.getComponent<Rigidbody2DComponent>();

				b2BodyId& body = m_entityBox2DBodyMap.at(enity.getUUID());
				const auto& position = b2Body_GetPosition(body);
				transform.position.x = position.x;
				transform.position.y = position.y;
				transform.rotation.z = b2Rot_GetAngle(b2Body_GetRotation(body));
			}
		}

		// Render2D sprites
		if (mainCamera)
		{
			graphics::Renderer2D::begin(*mainCamera, mainCameraTransform);

			{
				auto group = m_registry.group<TransformComponent, SpriteRendererComponent>();
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					graphics::Renderer2D::drawSprite(transform.getTransform(), sprite, (uint32_t)entity);
				}
			}

			{
				auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
				view.each([](const auto entity, const auto& transform, const auto& circleComp)
					{
						if (circleComp.thickness != 0.0f)
							graphics::Renderer2D::drawCircle(transform.getTransform(), circleComp.colour, circleComp.thickness, circleComp.fade, (uint32_t)entity);
					});
			}

			graphics::Renderer2D::end();
		}
	}

	void Scene::updateSimulation(const application::DeltaTime& dt, graphics::EditorCamera& camera)
	{
		// Physics
		{
			const int32_t subStepCount = 4;
			b2World_Step(*m_physicsWorld, dt, subStepCount);

			auto view = m_registry.view<Rigidbody2DComponent>();

			for (auto e : view)
			{
				Entity enity = { e, this };
				auto& transform = enity.getComponent<TransformComponent>();
				auto& rb2d = enity.getComponent<Rigidbody2DComponent>();

				b2BodyId& body = m_entityBox2DBodyMap.at(enity.getUUID());
				const auto& position = b2Body_GetPosition(body);
				transform.position.x = position.x;
				transform.position.y = position.y;
				transform.rotation.z = b2Rot_GetAngle(b2Body_GetRotation(body));
			}
		}

		// render
		renderSceneEditor(camera);
	}

	void Scene::updateEditor(const application::DeltaTime& dt, graphics::EditorCamera& camera)
	{
		// render
		renderSceneEditor(camera);
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

	void Scene::physics2DStart()
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { 0.0f, -9.8f };
		m_physicsWorld = create_uni_ptr<b2WorldId>(b2CreateWorld(&worldDef));

		using namespace scene;

		auto view = m_registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.getComponent<TransformComponent>();
			auto& rb2d = entity.getComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = rigid_body_type_to_box2D_body_type(rb2d.type);
			bodyDef.position = { transform.position.x, transform.position.y };
			bodyDef.rotation = b2MakeRot(transform.rotation.z);
			//bodyDef.fixedRotation = true;

			b2BodyId body = b2CreateBody(*m_physicsWorld, &bodyDef);

			if (rb2d.fixedRotation)
			{
				b2MotionLocks locks;
				locks.linearX = false;	// need to initialize this?
				locks.linearY = false;	// need to initialize this?
				locks.angularZ = true;
				b2Body_SetMotionLocks(body, locks);
			}

			m_entityBox2DBodyMap[entity.getUUID()] = body;

			if (entity.hasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.getComponent<BoxCollider2DComponent>();

				b2Polygon polygon = b2MakeOffsetBox(bc2d.size.x * transform.scale.x, bc2d.size.y * transform.scale.y, b2Vec2(bc2d.offset.x, bc2d.offset.y), b2MakeRot(0.0f));

				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = rb2d.desity;
				shapeDef.material.friction = rb2d.friction;
				shapeDef.material.restitution = rb2d.restitution;
				//shapeDef.material.restitutionThreshold = bc2d.restitutionThreshold;
				b2ShapeId shapeID = b2CreatePolygonShape(body, &shapeDef, &polygon);
			}

			if (entity.hasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.getComponent<CircleCollider2DComponent>();

				b2Circle circle;
				circle.center = b2Vec2(cc2d.offset.x, cc2d.offset.y);
				// This assumes circles are uniform
				circle.radius = cc2d.radius * transform.scale.x;

				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = rb2d.desity;
				shapeDef.material.friction = rb2d.friction;
				shapeDef.material.restitution = rb2d.restitution;

				b2ShapeId shapeID = b2CreateCircleShape(body, &shapeDef, &circle);
			}
		}
	}

	void Scene::physics2DStop()
	{
		if (m_physicsWorld)
		{
			b2DestroyWorld(*m_physicsWorld);
			m_physicsWorld.reset();
		}
	}

	void Scene::scriptingStart()
	{
		using namespace scripting;
		ScriptEngine::onRuntimeStart(this);
		
		auto view = m_registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			ScriptEngine::createEntityInstance(entity);
		}
	}

	void Scene::scriptingStop()
	{
		scripting::ScriptEngine::onRuntimeStop();
	}

	void Scene::renderSceneEditor(graphics::EditorCamera& camera)
	{
		// Render2D sprites
		graphics::Renderer2D::begin(camera);

		{
			auto group = m_registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				graphics::Renderer2D::drawSprite(transform.getTransform(), sprite, (uint32_t)entity);
			}
		}

		{
			auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
			view.each([](const auto entity, const auto& transform, const auto& circleComp)
				{
					if (circleComp.thickness != 0.0f)
						graphics::Renderer2D::drawCircle(transform.getTransform(), circleComp.colour, circleComp.thickness, circleComp.fade, (uint32_t)entity);
				});
		}

		graphics::Renderer2D::end();
	}

	// TO DO: figure a way to remove the below functions/make them optional
	// currently they have to have a template, even if its empty

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
	void scene::Scene::onComponentAdded<scene::CircleRendererComponent>(Entity entity, scene::CircleRendererComponent& component)
	{
	}

	template<>
	void scene::Scene::onComponentAdded<scene::Rigidbody2DComponent>(Entity entity, scene::Rigidbody2DComponent& component)
	{
	}

	template<>
	void scene::Scene::onComponentAdded<scene::BoxCollider2DComponent>(Entity entity, scene::BoxCollider2DComponent& component)
	{
	}

	template<>
	void scene::Scene::onComponentAdded<scene::CircleCollider2DComponent>(Entity entity, scene::CircleCollider2DComponent& component)
	{
	}

	template<>
	void scene::Scene::onComponentAdded<scene::NativeScriptComponent>(Entity entity, scene::NativeScriptComponent& component)
	{
	}

	template<>
	void scene::Scene::onComponentAdded<scene::ScriptComponent>(Entity entity, scene::ScriptComponent& component)
	{
	}

}