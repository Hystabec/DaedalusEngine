#include "ddpch.h"
#include "scriptGlue.h"

#include "scriptEngine.h"
#include "../application/input/Input.h"
#include "maths/vec3.h"
#include "../scene/scene.h"
#include "../scene/entity.h"
#include "../scene/entityComponents/components.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

namespace daedalus::scripting {

	static std::unordered_map<MonoType*, std::function<bool(scene::Entity)>> s_entityHasComponentFuncs;

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

	static bool entity_has_component(UUID uuid, MonoReflectionType* componentType)
	{
		using namespace scene;
		Scene* sceneContext = scripting::ScriptEngine::getSceneContext();
		DD_CORE_ASSERT(sceneContext);
		Entity entity = sceneContext->getEntityByUUID(uuid);
		DD_CORE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		DD_CORE_ASSERT(s_entityHasComponentFuncs.find(managedType) != s_entityHasComponentFuncs.end());
		return s_entityHasComponentFuncs.at(managedType)(entity);
	}

	static void transform_component_get_position(UUID uuid, maths::Vec3* outPosition)
	{
		using namespace scene;
		Scene* sceneContext = scripting::ScriptEngine::getSceneContext();
		DD_CORE_ASSERT(sceneContext);
		Entity entity = sceneContext->getEntityByUUID(uuid);
		DD_CORE_ASSERT(entity);
		*outPosition = entity.getTransformComponent().position;
	}

	static void transform_component_set_position(UUID uuid, maths::Vec3* inPosition)
	{
		using namespace scene;
		Scene* sceneContext = scripting::ScriptEngine::getSceneContext();
		DD_CORE_ASSERT(sceneContext);
		Entity entity = sceneContext->getEntityByUUID(uuid);
		DD_CORE_ASSERT(entity);
		entity.getTransformComponent().position = *inPosition;
	}

	template<typename... Component>
	static void register_component_types_base(MonoImage* assemblyImage)
	{
		([](MonoImage* assemblyImage)
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);

				std::string managedTypeName = std::format("Daedalus.Types.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), assemblyImage);

				if (!managedType)
				{
					DD_CORE_LOG_ERROR("Could not find component type '{}'", managedTypeName);
					return;
				}

				s_entityHasComponentFuncs[managedType] = [](scene::Entity entity) { return entity.hasComponent<Component>(); };
			}(assemblyImage), ...);
	}

	template<typename... Component>
	static void register_component_types_base(scene::ComponentGroup<Component...>, MonoImage* assemblyImage)
	{
		register_component_types_base<Component...>(assemblyImage);
	}

	void ScriptGlue::registerComponentTypes()
	{
		register_component_types_base(scene::AllComponents{}, ScriptEngine::getCoreAssemblyImage());
	}

	void ScriptGlue::registerFunctions()
	{
		DD_ADD_INTERNAL_CALL(native_log);

		DD_ADD_INTERNAL_CALL(input_get_key_up);
		DD_ADD_INTERNAL_CALL(input_get_key_down);

		DD_ADD_INTERNAL_CALL(entity_has_component);

		DD_ADD_INTERNAL_CALL(transform_component_get_position);
		DD_ADD_INTERNAL_CALL(transform_component_set_position);
	}

}