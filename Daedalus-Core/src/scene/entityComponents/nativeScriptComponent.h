#pragma once

#include "../scriptableEntity.h"

namespace daedalus::scene {

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;
		bool scriptBound = false;

		ScriptableEntity* (*instantiateScript)();
		void (*destroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind()
		{
			instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			destroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
			scriptBound = true;
		}
	};

}