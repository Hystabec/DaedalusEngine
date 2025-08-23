#include "ddpch.h"
#include "physicsScene2D.h"

#include "entity.h"
#include "entityComponents/transformComponent.h"
#include "entityComponents/rigidbody2DComponent.h"
#include "entityComponents/boxCollider2DComponent.h"
#include "entityComponents/circleCollider2DComponent.h"

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

	PhysicsScene2D::PhysicsScene2D(Scene* sceneContext)
		: m_sceneContext(sceneContext)
	{
	}

	void PhysicsScene2D::startScene()
	{
		DD_CORE_ASSERT(!m_physicsWorld, "duplicate physics world. 'endScene' not called?");

		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { 0.0f, -9.8f };
		m_physicsWorld = create_uni_ptr<b2WorldId>(b2CreateWorld(&worldDef));
	}

	void PhysicsScene2D::updateScene(const application::DeltaTime& dt)
	{
		const int32_t subStepCount = 4;
		b2World_Step(*m_physicsWorld, dt, subStepCount);

		for (auto& entityPair : m_entityBox2DBodyMap)
		{
			Entity entity =  m_sceneContext->getEntityByUUID(entityPair.first);
			auto& transform = entity.getComponent<TransformComponent>();
			auto& rb2d = entity.getComponent<Rigidbody2DComponent>();

			b2BodyId& body = entityPair.second;
			const auto& position = b2Body_GetPosition(body);
			transform.position.x = position.x;
			transform.position.y = position.y;
			transform.rotation.z = b2Rot_GetAngle(b2Body_GetRotation(body));
		}
	}

	void PhysicsScene2D::endScene()
	{
		if (m_physicsWorld)
		{
			b2DestroyWorld(*m_physicsWorld);
			m_physicsWorld.reset();
		}
	}

	void PhysicsScene2D::registerEntity(Entity entity)
	{
		DD_CORE_ASSERT(m_physicsWorld, "No physics world. 'startScene' not called?");

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