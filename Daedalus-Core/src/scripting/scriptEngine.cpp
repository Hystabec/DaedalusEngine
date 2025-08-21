#include "ddpch.h"
#include "scriptEngine.h"

#include "utils/findFileLocation.h"
#include "scriptGlue.h"

#include "../scene/scene.h"
#include "../scene/entity.h"
#include "../scene/entityComponents/scriptComponent.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

namespace daedalus::scripting {

	namespace monoUtils {

		// TO DO: Move this to file utils
		static char* read_bytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = (uint32_t)(end - stream.tellg());

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		static MonoAssembly* load_mono_assembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = read_bytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}

		static void print_assembly_types(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				DD_CORE_LOG_TRACE("{}.{}", nameSpace, name);
			}
		}

	}

	struct ScriptEngineData
	{
		MonoDomain* rootDomain = nullptr;
		MonoDomain* appDomain = nullptr;

		MonoAssembly* coreAssembly = nullptr;
		MonoImage* coreAssemblyImage = nullptr;

		ScriptClass entityClass;
		std::unordered_map<std::string, Shr_ptr<ScriptClass>> entityClasses;
		std::unordered_map<UUID, Shr_ptr<ScriptInstance>> entityInstances;

		scene::Scene* sceneContext = nullptr;
	};

	static ScriptEngineData* s_data = nullptr;

	void ScriptEngine::init()
	{
		s_data = new ScriptEngineData();
		
		initMono();
		loadAssembly("resources/scripts/Daedalus-ScriptCore.dll");
		loadAssemblyClasses(s_data->coreAssembly);
		
		ScriptGlue::registerFunctions();

		s_data->entityClass = ScriptClass("Daedalus.Types", "Entity");
#if 0
		// retive and instantiate class (with constuctor)
		// 1. create an object (and call constuctor)

		MonoObject* instance = s_data->entityClass.instantiate();

		// 2. call methods
		{
			MonoMethod* printMessageFunc = s_data->entityClass.getMethod("PrintMessage", 0);
			s_data->entityClass.invokeMethod(instance, printMessageFunc, nullptr);
		}

		// 3. call methods with param
		{
			MonoMethod* printIntFunc = s_data->entityClass.getMethod("PrintInt", 1);

			int value = 5;
			void* param = &value;

			s_data->entityClass.invokeMethod(instance, printIntFunc, &param);
		}
		{
			MonoMethod* printIntsFunc = s_data->entityClass.getMethod("PrintInts", 2);

			int value1 = 63;
			int value2 = 9321;
			void* params[] =
			{
				&value1,
				&value2
			};

			s_data->entityClass.invokeMethod(instance, printIntsFunc, params);
		}
		{
			MonoString* monoString = mono_string_new(s_data->appDomain, "Hello world from C++");

			MonoMethod* printCustomMessageFunc = s_data->entityClass.getMethod("PrintCustomMessage", 1);

			void* stringParam = monoString;

			s_data->entityClass.invokeMethod(instance, printCustomMessageFunc, &stringParam);
		}

		//DD_CORE_ASSERT(false);
#endif
	}

	void ScriptEngine::shutdown()
	{
		shutdownMono();

		delete s_data;
		s_data = nullptr;
	}

	void ScriptEngine::initMono()
	{
		{
			auto [path, result] = daedalus::utils::get_core_file_location("mono/lib");
			DD_CORE_ASSERT(result, "Mono folder not found");
			mono_set_assemblies_path(path.string().c_str());
		}

		MonoDomain* rootDomain = mono_jit_init("DaedalusJitRuntime");
		DD_CORE_ASSERT(rootDomain);

		// Store the root domain pointer
		s_data->rootDomain = rootDomain;
	}

	void ScriptEngine::shutdownMono()
	{
		// NOTE: mono is difficult to shutdown, revisit this

		//mono_domain_unload(s_data->appDomain);
		s_data->appDomain = nullptr;

		//mono_jit_cleanup(s_data->rootDomain);
		s_data->rootDomain = nullptr;
	}

	void ScriptEngine::loadAssembly(const std::filesystem::path& filepath)
	{
		// Create an App Domain
		s_data->appDomain = mono_domain_create_appdomain((char*)"DaedalusScriptRuntime", nullptr);
		mono_domain_set(s_data->appDomain, true);

		auto [path, result] = daedalus::utils::get_core_file_location(filepath);
		if (result)
		{
			s_data->coreAssembly = monoUtils::load_mono_assembly(path);
			//monoUtils::print_assembly_types(s_data->coreAssembly);
			s_data->coreAssemblyImage = mono_assembly_get_image(s_data->coreAssembly);
		}
		else
			DD_ASSERT(false, "failed to load assembly (file could not be found)");
	}

	void ScriptEngine::onRuntimeStart(scene::Scene* scene)
	{
		s_data->sceneContext = scene;
	}

	void ScriptEngine::onRuntimeStop()
	{
		s_data->sceneContext = nullptr;
		s_data->entityInstances.clear();
	}

	bool ScriptEngine::entityClassExists(const std::string& fullClassName)
	{
		return s_data->entityClasses.find(fullClassName) != s_data->entityClasses.end();
	}

	void ScriptEngine::createEntityInstance(scene::Entity entity)
	{
		const auto& sc = entity.getComponent<scene::ScriptComponent>();
		if (ScriptEngine::entityClassExists(sc.className))
		{
			Shr_ptr<ScriptInstance> instance = create_shr_ptr<ScriptInstance>(s_data->entityClasses[sc.className], entity);

			s_data->entityInstances[entity.getUUID()] = instance;

			instance->invokeOnCreate();
		}
	}

	void ScriptEngine::updateEntityInstance(scene::Entity entity, float dt)
	{
		UUID entityUUID = entity.getUUID();
		DD_CORE_ASSERT(s_data->entityInstances.find(entityUUID) != s_data->entityInstances.end());

		Shr_ptr<ScriptInstance> instance = s_data->entityInstances[entityUUID];

		instance->invokeOnUpdate(dt);
	}

	scene::Scene* ScriptEngine::getSceneContext()
	{
		return s_data->sceneContext;
	}

	const std::unordered_map<std::string, Shr_ptr<ScriptClass>>& ScriptEngine::getEntityClasses()
	{
		return s_data->entityClasses;
	}

	MonoObject* ScriptEngine::instantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_data->appDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	void ScriptEngine::loadAssemblyClasses(MonoAssembly* assembly)
	{
		s_data->entityClasses.clear();

		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* baseEntityClass = mono_class_from_name(image, "Daedalus.Types", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);
			if (monoClass == baseEntityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, baseEntityClass, false);

			if (isEntity)
			{
				std::string fullName;
				if (strlen(nameSpace) != 0)
					fullName = std::format("{}.{}", nameSpace, name);
				else
					fullName = name;

				s_data->entityClasses[fullName] = create_shr_ptr<ScriptClass>(nameSpace, name);

				DD_CORE_LOG_TRACE("{}.{} is a subclass of Daedalus.Entity", nameSpace, name);
			}
		}
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
		: m_classNamespace(classNamespace), m_className(className)
	{
		m_monoClass = mono_class_from_name(s_data->coreAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::instantiate()
	{
		return ScriptEngine::instantiateClass(m_monoClass);
	}

	MonoMethod* ScriptClass::getMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_monoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::invokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	ScriptInstance::ScriptInstance(Shr_ptr<ScriptClass> scriptClass, scene::Entity entity)
		: m_scriptClass(scriptClass)
	{
		m_instance = scriptClass->instantiate();

		m_constuctor = s_data->entityClass.getMethod(".ctor", 1);
		m_onStartMethod = scriptClass->getMethod("OnStart", 0);
		m_onUpdateMethod = scriptClass->getMethod("OnUpdate", 1);

		// call constuctor
		UUID uuid = entity.getUUID();
		void* param = &uuid;
		m_scriptClass->invokeMethod(m_instance, m_constuctor, &param);
	}

	void ScriptInstance::invokeOnCreate()
	{
		m_scriptClass->invokeMethod(m_instance, m_onStartMethod);
	}

	void ScriptInstance::invokeOnUpdate(float dt)
	{
		void* param = &dt;
		m_scriptClass->invokeMethod(m_instance, m_onUpdateMethod, &param);
		
	}

}