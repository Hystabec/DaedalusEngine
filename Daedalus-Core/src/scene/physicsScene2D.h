#pragma once

#include "../application/uuid.h"
#include "../application/time/deltaTime.h"

#include "../maths/vec2.h"

struct b2WorldId;
struct b2BodyId;

namespace daedalus::scene {

	class Entity;
	class Scene;

	class PhysicsScene2D
	{
	public:
		PhysicsScene2D(Scene* sceneContext);

		void startScene();
		void updateScene(const application::DeltaTime& dt);
		void endScene();

		void applyForceToEntity(Entity entity, maths::Vec2 force, maths::Vec2 worldPoint, bool wake);
		void applyForceCentreToEntity(Entity entity, maths::Vec2 force, bool wake);
		void applyLinearImpulseToEntity(Entity entity, maths::Vec2 impulse, maths::Vec2 worldPoint, bool wake);
		void applyLinearImpulseCentreToEntity(Entity entity, maths::Vec2 impulse, bool wake);
		void applyTorqueToEntity(Entity entity, float torque, bool wake);

		void registerEntity(Entity entity);

	private:
		bool bodyMapContainsEntity(Entity entity);

	private:
		// This being a unique ptr is a bit weird, consider changing later
		Scene* m_sceneContext = nullptr;
		Uni_ptr<b2WorldId> m_physicsWorld = nullptr;
		std::unordered_map<daedalus::UUID, b2BodyId> m_entityBox2DBodyMap;
	};

}