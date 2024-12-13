#pragma once

#include "core.h"
#include "graphics/window.h"

#include "events/windowEvent.h"
#include "app/layerStack.h"

namespace daedalusCore {

	class DD_API Application
	{
	private:
		std::unique_ptr<graphics::Window> m_window;

		//unsigned int m_FramesPerSecond = 0;
		//unsigned int m_UpdatesPerSecond = 0;

		bool m_running = true;

		app::layerStack m_layerStack;
	private:
		bool OnWindowClose(event::WindowClosedEvent& e);

	protected:
		Application(std::string title = "Daedalus Window", unsigned int width = 960, unsigned int height = 540, bool vsync = false);

	public:
		virtual ~Application();

		void OnEvent(event::Event& e);

		void Init();
		int Run();

		void PushLayer(app::Layer* layer);
		void PushOverlay(app::Layer* layer);

		//inline unsigned int GetFramesPerSecond() const { return m_FramesPerSecond; }
		//inline unsigned int GetUpdatesPerSecond() const { return m_UpdatesPerSecond; }
	};

	//To be defined in client
	Application* CreateApplication();

}