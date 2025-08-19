#pragma once

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
}

namespace daedalus::scripting {

	class ScriptEngine
	{
	public:
		static void init();
		static void shutdown();

		static void loadAssembly(const std::filesystem::path& filepath);
	private:
		static void initMono();
		static void shutdownMono();

		static MonoObject* instantiateClass(MonoClass* monoClass);

		friend class ScriptClass;
	};

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

}