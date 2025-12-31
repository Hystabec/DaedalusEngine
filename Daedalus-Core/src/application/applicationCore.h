#pragma once

#include "application/window.h"

#include "application/time/time.h"

#include "events/windowEvent.h"
#include "application/layers/layerStack.h"
#include "application/layers/imGuiLayer.h"

namespace daedalus {

	struct ApplicationCommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int index) const
		{
			DD_CORE_ASSERT(index < count);
			return args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string name = "Daedalus Window";
		uint32_t width = 960;
		uint32_t height = 540;
		bool vsync = false;
		std::filesystem::path workingDirectory;
		ApplicationCommandLineArgs commandLineArgs = ApplicationCommandLineArgs();
	};

	class  Application
	{
	public:
		virtual ~Application();
		inline static Application& get() { return *s_instance; }

		void onEvent(event::Event& e);

		void init();
		int run();

		void pushLayer(application::Layer* layer);
		void pushOverlay(application::Layer* layer);

		inline application::Window* getWindow() { return m_window.get(); }

		void close();

		inline application::ImGuiLayer* getImGuiLayer() { return m_ImGuiLayer; }

		const ApplicationSpecification& getSpecification() const { return m_specification; }

		void submitToMainThreadQueue(const std::function<void()>& function);
	protected:
		Application(const ApplicationSpecification& specification);

	private:
		bool onWindowClose(event::WindowClosedEvent& e);
		bool onWindowResize(event::WindowResizedEvent& e);
		void executeMainThreadQueue();

	private:
		static Application* s_instance;
		
		ApplicationSpecification m_specification;
		ScopedPtr<application::Window> m_window;
		application::ImGuiLayer* m_ImGuiLayer;
		bool m_running = true;
		bool m_minimized = false;
		application::LayerStack m_layerStack;

		float m_lastFrameTime = 0.0f;
		application::Time m_time;

		std::vector<std::function<void()>> m_mainThreadQueue;
		std::mutex m_mainThreadQueueMutex;
	};

	//To be defined in client
	Application* createApplication(ApplicationCommandLineArgs args);

}