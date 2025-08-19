#pragma once

namespace daedalus::scripting {

	class ScriptEngine
	{
	public:
		static void init();
		static void shutdown();
	private:
		static void initMono();
		static void shutdownMono();
	};

}