#include "ddpch.h"
#include "scriptEngine.h"

#include "utils/findFileLocation.h"
#include "scriptGlue.h"

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
	};

	static ScriptEngineData* s_data = nullptr;

	void ScriptEngine::init()
	{
		s_data = new ScriptEngineData();
		initMono();
		loadAssembly("resources/scripts/Daedalus-ScriptCore.dll");
		ScriptGlue::registerFunctions();

		// retive and instantiate class (with constuctor)
		// 1. create an object (and call constuctor)
		s_data->entityClass = ScriptClass("daedalus", "Entity");

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

	MonoObject* ScriptEngine::instantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_data->appDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
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

}