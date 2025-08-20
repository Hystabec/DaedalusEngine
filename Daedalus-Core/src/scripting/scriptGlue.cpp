#include "ddpch.h"
#include "scriptGlue.h"

#include "scriptEngine.h"
#include "../application/input/Input.h"
#include "maths/vec3.h"
#include "../scene/scene.h"
#include "../scene/entity.h"

#include <mono/jit/jit.h>

namespace daedalus::scripting {

#define DD_ADD_INTERNAL_CALL(FuncName) mono_add_internal_call("Daedalus.InternalCalls::" #FuncName, FuncName)

	static void native_log(MonoString* string)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);

		DD_LOG_TRACE(str);
	}

	static bool input_get_key_up(application::InputCode inputCode)
	{
		return application::Input::getKeyUp(inputCode);
	}

	static bool input_get_key_down(application::InputCode inputCode)
	{
		return application::Input::getKeyDown(inputCode);
	}

	static void entity_get_position(UUID uuid, maths::Vec3* outPosition)
	{
		using namespace scene;
		Scene* sceneContext = scripting::ScriptEngine::getSceneContext();
		Entity entity = sceneContext->getEntityByUUID(uuid);
		*outPosition = entity.getTransformComponent().position;
	}

	static void entity_set_position(UUID uuid, maths::Vec3* inPosition)
	{
		using namespace scene;
		Scene* sceneContext = scripting::ScriptEngine::getSceneContext();
		Entity entity = sceneContext->getEntityByUUID(uuid);
		entity.getTransformComponent().position = *inPosition;
	}

	void ScriptGlue::registerFunctions()
	{
		DD_ADD_INTERNAL_CALL(native_log);
		DD_ADD_INTERNAL_CALL(input_get_key_up);
		DD_ADD_INTERNAL_CALL(input_get_key_down);
		DD_ADD_INTERNAL_CALL(entity_get_position);
		DD_ADD_INTERNAL_CALL(entity_set_position);
	}

}