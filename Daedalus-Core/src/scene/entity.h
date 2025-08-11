#pragma once

#include "scene.h"
#include "application/uuid.h"

#include "entityComponents/idComponent.h"
#include "entityComponents/transformComponent.h"
#include "entityComponents/tagComponent.h"

#include <entt.hpp>

namespace daedalus::scene {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, daedalus::scene::Scene* scene);
		Entity(const Entity& other) = default;

		/// @brief Add a specified component to the entity. 
		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			DD_CORE_ASSERT(!hasComponent<T>(), "Entity already has component");
			T& component = m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
			m_scene->onComponentAdded<T>(*this, component);
			return component;
		}

		/// @brief Add or replace a specified component on the entity. 
		template<typename T, typename... Args>
		T& addOrRepalaceComponent(Args&&... args)
		{
			DD_CORE_ASSERT(m_scene, "Scene is nullptr (invalid entity)");
			T& component = m_scene->m_registry.emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
			m_scene->onComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& getComponent()
		{
			DD_CORE_ASSERT(hasComponent<T>(), "Entity does not has component");
			return m_scene->m_registry.get<T>(m_handle);
		}

		template<typename T>
		const T& getComponent() const
		{
			DD_CORE_ASSERT(hasComponent<T>(), "Entity does not has component");
			return m_scene->m_registry.get<T>(m_handle);
		}

		template<typename... T>
		bool hasComponent() const
		{
			DD_CORE_ASSERT(m_scene, "Scene is nullptr (invalid entity)");
			return m_scene->m_registry.all_of<T...>(m_handle);
		}

		/// @brief Remove a specified component from the entity. 
		/// @brief Trying to remove IDComponent, TagComponent or TransformComponent will cause an assert
		template<typename T>
		void removeComponent()// requires(!std::is_same<T, TransformComponent>::value && !std::is_same<T, TagComponent>::value)
		{
			DD_CORE_ASSERT(hasComponent<T>(), "Entity does not has component");
			DD_CORE_ASSERT(!(typeid(T).hash_code() == typeid(IDComponent).hash_code() || 
				typeid(T).hash_code() == typeid(TagComponent).hash_code()) ||
				typeid(T).hash_code() == typeid(TransformComponent).hash_code(),
				DD_ASSERT_FORMAT_MESSAGE("Cannot remove component [{}]", typeid(T).name()));
			m_scene->m_registry.remove<T>(m_handle);
		}

		UUID getUUID() const { return getComponent<IDComponent>().ID; }
		const std::string& getName() const { return getComponent<IDComponent>().name; }
		TransformComponent& getTransformComponent() { return getComponent<TransformComponent>(); }

		operator bool() const { return m_handle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_handle; }
		operator entt::entity() const { return m_handle; }

		bool operator==(const Entity& other) const { return m_handle == other.m_handle && m_scene == other.m_scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_handle{ entt::null };
		Scene* m_scene = nullptr;
	};

}