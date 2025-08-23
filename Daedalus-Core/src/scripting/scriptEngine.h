#pragma once

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
}

namespace daedalus::scene {
	class Scene;
	class Entity;
}

namespace daedalus::scripting {

	class ScriptClass
	{
	public:
		ScriptClass() = default;

		ScriptClass(const std::string& classNamespace, const std::string& className);


		MonoObject* instantiate();
		MonoMethod* getMethod(const std::string& name, int parameterCount);
		MonoObject* invokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

	private:
		std::string m_classNamespace;
		std::string m_className;
		MonoClass* m_monoClass = nullptr;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Shr_ptr<ScriptClass> scriptClass, scene::Entity entity);

		void invokeOnStart();
		void invokeOnUpdate(float dt);
	private:
		Shr_ptr<ScriptClass> m_scriptClass;
		MonoObject* m_instance = nullptr;
		MonoMethod* m_constuctor = nullptr;
		MonoMethod* m_onStartMethod = nullptr;
		MonoMethod* m_onUpdateMethod = nullptr;
	};

	class ScriptEngine
	{
	public:
		static void init();
		static void shutdown();

		static void loadAssembly(const std::filesystem::path& filepath);
		static void onRuntimeStart(scene::Scene* scene);
		static void onRuntimeStop();

		static bool entityClassExists(const std::string& fullClassName);
		static void createEntityInstance(scene::Entity entity);
		static void updateEntityInstance(scene::Entity entity, float dt);

		static scene::Scene* getSceneContext();
		static const std::unordered_map<std::string, Shr_ptr<ScriptClass>>& getEntityClasses();
	private:
		static void initMono();
		static void shutdownMono();

		static MonoObject* instantiateClass(MonoClass* monoClass);
		static void loadAssemblyClasses(MonoAssembly* assembly);

		static MonoImage* getCoreAssemblyImage();

		friend class ScriptClass;
		friend class ScriptGlue;
	};

}