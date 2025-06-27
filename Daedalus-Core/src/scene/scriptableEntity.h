#pragma once
#include "entity.h"

namespace daedalus::scene {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& getComponent()
		{
			return m_entity.getComponent<T>();
		}

	protected:
		virtual void onCreate() {}
		virtual void onDestroy() {}
		virtual void onUpdate(const application::DeltaTime& dt) {}

	private:
		Entity m_entity;
		friend class Scene;
	};

}