#pragma once

#include "core.h"
#include "application/window.h"

#include "events/windowEvent.h"
#include "application/layers/layerStack.h"

namespace daedalusCore {

	class DD_API Application
	{
	private:
		static Application* m_instance;

		std::unique_ptr<application::Window> m_window;

		bool m_running = true;

		application::layerStack m_layerStack;
	private:
		bool OnWindowClose(event::WindowClosedEvent& e);

	protected:
		Application(std::string title = "Daedalus Window", unsigned int width = 960, unsigned int height = 540, bool vsync = false);

	public:
		virtual ~Application();
		inline static Application& Get() { return *m_instance; }

		void OnEvent(event::Event& e);

		void Init();
		int Run();

		void PushLayer(application::Layer* layer);
		void PushOverlay(application::Layer* layer);

		inline application::Window* GetWindow() { return m_window.get(); }
	};

	//To be defined in client
	Application* CreateApplication();

}