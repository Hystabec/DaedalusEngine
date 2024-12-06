#pragma once

#include "core.h"

namespace daedalusCore {

	class DD_API Application
	{
	private:
		const char* m_applicationName = "Daedalus Application";
		unsigned int m_FramesPerSecond = 0;
		unsigned int m_UpdatesPerSecond = 0;

	protected:
		Application(const char* applicationName);

	public:
		virtual ~Application();

		//Runs on initialization
		virtual void Init() {};
		//Runs on close
		virtual void Close() {};
		//Run once per second
		virtual void Tick() {};
		//Runs 60 times per second
		virtual void Update() {};
		//Runs as fast as possible
		virtual void Render() {};

		int Run();

		inline unsigned int GetFramesPerSecond() const { return m_FramesPerSecond; }
		inline unsigned int GetUpdatesPerSecond() const { return m_UpdatesPerSecond; }
	};

	//To be defined in client
	Application* CreateApplication();

}