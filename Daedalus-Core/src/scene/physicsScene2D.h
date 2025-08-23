#pragma once

#include "../application/uuid.h"
#include "../application/time/deltaTime.h"

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

		void registerEntity(Entity entity);

	private:
		// This being a unique ptr is a bit weird, consider changing later
		Scene* m_sceneContext = nullptr;
		Uni_ptr<b2WorldId> m_physicsWorld = nullptr;
		std::unordered_map<daedalus::UUID, b2BodyId> m_entityBox2DBodyMap;
	};

}