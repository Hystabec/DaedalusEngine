#pragma once

#include "../application/uuid.h"

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

	enum class ScriptFieldType
	{
		None = 0,
		Bool, Float, Double,
		Char, Byte,
		Short, UShort,
		Int, UInt,
		Long, ULong,
		string,
		Vector2, Vector3, Vector4,
		MonoScript
	};

	struct ScriptField
	{
		ScriptFieldType type = ScriptFieldType::None;
		std::string name = "";
		MonoClassField* classField = nullptr;
	};

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
			bool success = getFieldValueInternal(name, s_fieldValueBuffer);

			if (!success)
				return T();

			return *((T*)s_fieldValueBuffer);
		}

		template<typename T>
		void setFieldValue(const std::string& name, const T& value)
		{
			setFieldValueInternal(name, &value);
		}
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
		inline static char s_fieldValueBuffer[sizeof(float)];
	};

	class ScriptEngine
	{
	public:
		static void init();
		static void shutdown();

		static void loadAssembly(const std::filesystem::path& filepath);
		static void loadClientAssembly(const std::filesystem::path& filepath);
		static void onRuntimeStart(scene::Scene* scene);
		static void onRuntimeStop();

		static bool entityClassExists(const std::string& fullClassName);
		static void createEntityInstance(scene::Entity entity);
		static void updateEntityInstance(scene::Entity entity, float dt);

		static scene::Scene* getSceneContext();
		static Shr_ptr<ScriptInstance> getEntityScriptInstance(daedalus::UUID entityID);
		static const std::unordered_map<std::string, Shr_ptr<ScriptClass>>& getEntityClasses();
	private:
		static void initMono();
		static void shutdownMono();

		static MonoObject* instantiateClass(MonoClass* monoClass);
		static void loadAssemblyClasses();

		static MonoImage* getCoreAssemblyImage();

		friend class ScriptClass;
		friend class ScriptGlue;
	};

}