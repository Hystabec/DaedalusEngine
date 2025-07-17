#pragma once

#include "../scriptableEntity.h"

namespace daedalus::scene {

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		bool ScriptBound = false;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
			ScriptBound = true;
		}
	};

}