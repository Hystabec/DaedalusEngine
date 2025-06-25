#include "ddpch.h"
#include "entity.h"

namespace daedalus::scene {

	Entity::Entity(entt::entity handle, daedalus::scene::Scene* scene)
		: m_handle(handle), m_scene(scene)
	{
	}

}