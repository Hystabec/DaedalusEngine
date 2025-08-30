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
#include <mono/metadata/tabledefs.h>

namespace daedalus::scripting {

	static const std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
	{
		{ "System.Boolean", ScriptFieldType::Bool },
		{ "System.Single", ScriptFieldType::Float },
		{ "System.Double", ScriptFieldType::Double },
		{ "System.Char", ScriptFieldType::Char },
		{ "System.Byte", ScriptFieldType::Byte },
		{ "System.Int16", ScriptFieldType::Short },
		{ "System.UInt16", ScriptFieldType::UShort },
		{ "System.Int32", ScriptFieldType::Int },
		{ "System.UInt32", ScriptFieldType::UInt },
		{ "System.Int64", ScriptFieldType::Long },
		{ "System.UInt64", ScriptFieldType::ULong },
		{ "System.String", ScriptFieldType::String },
		{ "Daedalus.Types.Vector2", ScriptFieldType::Vector2 },
		{ "Daedalus.Types.Vector3", ScriptFieldType::Vector3 },
		{ "Daedalus.Types.Vector4", ScriptFieldType::Vector4 },
		{ "Daedalus.Types.MonoScript", ScriptFieldType::MonoScript }
	};

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

		static ScriptFieldType mono_type_to_script_field_type(MonoType* monoType)
		{
			std::string typeName = mono_type_get_name(monoType);

			auto it = s_ScriptFieldTypeMap.find(typeName);

			if (it == s_ScriptFieldTypeMap.end())
			{
				DD_CORE_LOG_WARN("Script Engine: Unknown type: '{}'", typeName);
				return ScriptFieldType::None;
			}

			return it->second;
		}
	}

	struct ScriptEngineData
	{
		MonoDomain* rootDomain = nullptr;
		MonoDomain* appDomain = nullptr;

		MonoAssembly* coreAssembly = nullptr;
		MonoImage* coreAssemblyImage = nullptr;

		MonoAssembly* clientAssembly = nullptr;
		MonoImage* clientAssemblyImage = nullptr;

		ScriptClass entityClass;
		std::unordered_map<std::string, Shr_ptr<ScriptClass>> entityClasses;
		std::unordered_map<UUID, Shr_ptr<ScriptInstance>> entityInstances;
		
		std::unordered_map<UUID, ScriptFieldMap> entityScriptFields;

		scene::Scene* sceneContext = nullptr;
	};

	static ScriptEngineData* s_data = nullptr;

	void ScriptEngine::init()
	{
		s_data = new ScriptEngineData();
		
		initMono();

		loadAssembly("resources/script-bin/Daedalus-ScriptCore.dll");

		// NOTE: This will need to be made more expanable in the future, project DIR as arg(?)
		// but it is fine for testing currently (24/08/25)
		loadClientAssembly("sandboxProject/assets/scripts/script-bin/Sandbox.dll");
		loadAssemblyClasses();
		
		ScriptGlue::registerFunctions();
		ScriptGlue::registerComponentTypes();

		s_data->entityClass = ScriptClass("Daedalus.Types", "MonoScript", true);
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
			s_data->coreAssemblyImage = mono_assembly_get_image(s_data->coreAssembly);
			//monoUtils::print_assembly_types(s_data->coreAssembly);
		}
		else
			DD_ASSERT(false, "failed to load assembly (file could not be found)");
	}

	void ScriptEngine::loadClientAssembly(const std::filesystem::path& filepath)
	{

		s_data->clientAssembly = monoUtils::load_mono_assembly(filepath);
		s_data->clientAssemblyImage = mono_assembly_get_image(s_data->clientAssembly);
		//monoUtils::print_assembly_types(s_data->clientAssembly);
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

			UUID entityID = entity.getUUID();
			s_data->entityInstances[entityID] = instance;

			// copy feild values
			if (s_data->entityScriptFields.contains(entityID)) 
			{
				ScriptFieldMap& fieldMap = s_data->entityScriptFields.at(entityID);
				for (auto& [name, fieldInstance] : fieldMap)
				{
					instance->setFieldValueInternal(name, fieldInstance.m_fieldValueBuffer);
				}
			}

			instance->invokeOnStart();
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

	Shr_ptr<ScriptInstance> ScriptEngine::getEntityScriptInstance(daedalus::UUID entityID)
	{
		auto it = s_data->entityInstances.find(entityID);
		if (it == s_data->entityInstances.end())
			return nullptr;

		return it->second;
	}

	Shr_ptr<ScriptClass> ScriptEngine::getEntityClass(const std::string& className)
	{
		auto it = s_data->entityClasses.find(className);
		if (it == s_data->entityClasses.end())
			return nullptr;

		return it->second;
	}

	const std::unordered_map<std::string, Shr_ptr<ScriptClass>>& ScriptEngine::getEntityClasses()
	{
		return s_data->entityClasses;
	}

	ScriptFieldMap& ScriptEngine::getEntityScriptFields(daedalus::UUID entityID)
	{
		/*auto it = s_data->entityScriptFields.find(entityID);
		if (it == s_data->entityScriptFields.end())
		{
			DD_CORE_ASSERT(false);
			return ScriptFieldMap();
		}

		return it->second;*/
		return s_data->entityScriptFields[entityID];
	}

	MonoObject* ScriptEngine::instantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_data->appDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	void ScriptEngine::loadAssemblyClasses()
	{
		s_data->entityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_data->clientAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* baseEntityClass = mono_class_from_name(s_data->coreAssemblyImage, "Daedalus.Types", "MonoScript");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_data->clientAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(s_data->clientAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			MonoClass* monoClass = mono_class_from_name(s_data->clientAssemblyImage, nameSpace, className);
			if (monoClass == baseEntityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, baseEntityClass, false);

			if (!isEntity)
				continue;
			
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = std::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			Shr_ptr<ScriptClass> scriptClass = create_shr_ptr<ScriptClass>(nameSpace, className);
			s_data->entityClasses[fullName] = scriptClass;
			//DD_CORE_LOG_TRACE("{}.{} is a subclass of Daedalus.Types.MonoScript", nameSpace, className);

			int fieldCount = mono_class_num_fields(monoClass);
			//DD_CORE_LOG_TRACE("{} fields[{}]:", className, fieldCount);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					
					ScriptFieldType fieldType = monoUtils::mono_type_to_script_field_type(type);
					//DD_CORE_LOG_TRACE("  {} ({})", fieldName, utils::script_field_type_to_string(fieldType));
					scriptClass->m_fields[fieldName] = { fieldType, fieldName, field };
				}
			}
		}
	}

	MonoImage* ScriptEngine::getCoreAssemblyImage()
	{
		return s_data->coreAssemblyImage;
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isFromCore)
		: m_classNamespace(classNamespace), m_className(className)
	{
		m_monoClass = mono_class_from_name(isFromCore ? s_data->coreAssemblyImage : s_data->clientAssemblyImage, classNamespace.c_str(), className.c_str());
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

	void ScriptInstance::invokeOnStart()
	{
		if(m_onStartMethod)
			m_scriptClass->invokeMethod(m_instance, m_onStartMethod);
	}

	void ScriptInstance::invokeOnUpdate(float dt)
	{
		if (m_onUpdateMethod)
		{
			void* param = &dt;
			m_scriptClass->invokeMethod(m_instance, m_onUpdateMethod, &param);
		}
		
	}

	bool ScriptInstance::getFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_scriptClass->getFields();

		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_instance, field.classField, buffer);
		return true;
	}

	bool ScriptInstance::setFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_scriptClass->getFields();

		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_set_value(m_instance, field.classField, (void*)value);
		return true;
	}

}