#pragma once

#include "../application/uuid.h"
#include "maths/vec4.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace daedalus::scene {
	class Scene;
	class Entity;
}

namespace daedalus::scripting {

	namespace monoUtils
	{
		constexpr size_t maxFieldSize = sizeof(std::string);
	}

	enum class ScriptFieldType
	{
		None = 0,
		Bool, Float, Double,
		Char, Byte,
		Short, UShort,
		Int, UInt,
		Long, ULong,
		String,
		Vector2, Vector3, Vector4,
		MonoScript
	};

	struct ScriptField
	{
		ScriptFieldType type = ScriptFieldType::None;
		std::string name = "";
		MonoClassField* classField = nullptr;
	};

	// ScriptField + storage
	struct ScriptFieldInstance
	{
		ScriptField field;

		ScriptFieldInstance()
		{
			memset(m_fieldValueBuffer, 0, sizeof(m_fieldValueBuffer));
		}

		template<typename T>
		T getFieldValue()
		{
			static_assert(sizeof(T) <= monoUtils::maxFieldSize, "Type too large");

			return *((T*)m_fieldValueBuffer);
		}

		template<typename T>
		void setFieldValue(T value)
		{
			static_assert(sizeof(T) <= monoUtils::maxFieldSize, "Type too large");

			memcpy(m_fieldValueBuffer, &value, sizeof(T));
		}

	private:
		uint8_t m_fieldValueBuffer[monoUtils::maxFieldSize];

		friend class ScriptEngine;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class ScriptClass
	{
	public:
		ScriptClass() = default;

		ScriptClass(const std::string& classNamespace, const std::string& className, bool isFromCore = false);

		MonoObject* instantiate();
		MonoMethod* getMethod(const std::string& name, int parameterCount);
		MonoObject* invokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		inline const std::unordered_map<std::string, ScriptField>& getFields() { return m_fields; }
	private:
		std::string m_classNamespace;
		std::string m_className;
		
		std::unordered_map<std::string, ScriptField> m_fields;

		MonoClass* m_monoClass = nullptr;

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Shr_ptr<ScriptClass> scriptClass, scene::Entity entity);

		void invokeOnStart();
		void invokeOnUpdate(float dt);

		inline Shr_ptr<ScriptClass> getScriptClass() { return m_scriptClass; }

		template<typename T>
		T getFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= monoUtils::maxFieldSize, "Type too large");

			bool success = getFieldValueInternal(name, s_fieldValueBuffer);

			if (!success)
				return T();

			return *((T*)s_fieldValueBuffer);
		}

		template<typename T>
		void setFieldValue(const std::string& name, T value)
		{
			static_assert(sizeof(T) <= monoUtils::maxFieldSize, "Type too large");

			setFieldValueInternal(name, &value);
		}

		MonoObject* getManagedObject() { return m_instance; }
	private:
		bool getFieldValueInternal(const std::string& name, void* buffer);
		bool setFieldValueInternal(const std::string& name, const void* value);

	private:
		Shr_ptr<ScriptClass> m_scriptClass;
		MonoObject* m_instance = nullptr;
		MonoMethod* m_constuctor = nullptr;
		MonoMethod* m_onStartMethod = nullptr;
		MonoMethod* m_onUpdateMethod = nullptr;

		// buffer with the size of the largest supported type
		inline static uint8_t s_fieldValueBuffer[monoUtils::maxFieldSize];

		friend class ScriptEngine;
	};

	class ScriptEngine
	{
	public:
		static void init();
		static void shutdown();

		static void loadAssembly(const std::filesystem::path& filepath);
		static void loadClientAssembly(const std::filesystem::path& filepath);
		static void reloadAssembly();

		static void onRuntimeStart(scene::Scene* scene);
		static void onRuntimeStop();

		static bool entityClassExists(const std::string& fullClassName);
		static void createEntityInstance(scene::Entity entity);
		static void updateEntityInstance(scene::Entity entity, float dt);

		static scene::Scene* getSceneContext();
		static Shr_ptr<ScriptInstance> getEntityScriptInstance(daedalus::UUID entityID);
		static Shr_ptr<ScriptClass> getEntityClass(const std::string& className);
		static const std::unordered_map<std::string, Shr_ptr<ScriptClass>>& getEntityClasses();
		static ScriptFieldMap& getEntityScriptFields(daedalus::UUID entityID);

		static MonoObject* getManagedInstance(daedalus::UUID uuid, std::string_view instanceName);
	private:
		static void initMono();
		static void shutdownMono();

		static MonoObject* instantiateClass(MonoClass* monoClass);
		static void loadAssemblyClasses();

		static MonoImage* getCoreAssemblyImage();

		friend class ScriptClass;
		friend class ScriptGlue;
		friend struct ScriptFieldInstance;
	};

	namespace utils
	{
		inline const char* script_field_type_to_string(ScriptFieldType fieldType)
		{
			switch (fieldType)
			{
			case ScriptFieldType::None:
				return "None";
			case ScriptFieldType::Bool:
				return "Bool";
			case ScriptFieldType::Float:
				return "Float";
			case ScriptFieldType::Double:
				return "Double";
			case ScriptFieldType::Char:
				return "Char";
			case ScriptFieldType::Byte:
				return "Byte";
			case ScriptFieldType::Short:
				return "Short";
			case ScriptFieldType::UShort:
				return "UShort";
			case ScriptFieldType::Int:
				return "Int";
			case ScriptFieldType::UInt:
				return "UInt";
			case ScriptFieldType::Long:
				return "Long";
			case ScriptFieldType::ULong:
				return "ULong";
			case ScriptFieldType::String:
				return "String";
			case ScriptFieldType::Vector2:
				return "Vector2";
			case ScriptFieldType::Vector3:
				return "Vector3";
			case ScriptFieldType::Vector4:
				return "Vector4";
			case ScriptFieldType::MonoScript:
				return "MonoScript";
			}

			DD_CORE_ASSERT(false, "Unknow ScriptFieldType");
			return "None";
		}

		inline ScriptFieldType script_field_type_from_string(std::string_view fieldType)
		{
			if (fieldType == "None")
				return ScriptFieldType::None;
			else if (fieldType == "Bool")
				return ScriptFieldType::Bool;
			else if (fieldType == "Float")
				return ScriptFieldType::Float;
			else if (fieldType == "Double")
				return ScriptFieldType::Double;
			else if (fieldType == "Char")
				return ScriptFieldType::Char;
			else if (fieldType == "Byte")
				return ScriptFieldType::Byte;
			else if (fieldType == "Short")
				return ScriptFieldType::Short;
			else if (fieldType == "UShort")
				return ScriptFieldType::UShort;
			else if (fieldType == "Int")
				return ScriptFieldType::Int;
			else if (fieldType == "UInt")
				return ScriptFieldType::UInt;
			else if (fieldType == "Long")
				return ScriptFieldType::Long;
			else if (fieldType == "ULong")
				return ScriptFieldType::ULong;
			else if (fieldType == "String")
				return ScriptFieldType::String;
			else if (fieldType == "Vector2")
				return ScriptFieldType::Vector2;
			else if (fieldType == "Vector3")
				return ScriptFieldType::Vector3;
			else if (fieldType == "Vector4")
				return ScriptFieldType::Vector4;
			else if (fieldType == "MonoScript")
				return ScriptFieldType::MonoScript;

			DD_CORE_ASSERT(false, "Unknow ScriptFieldType");
			return ScriptFieldType::None;
		}
	}

}