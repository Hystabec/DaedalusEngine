#pragma once

#include "core.h"
#include "application/window.h"

#include "application/time/time.h"

#include "events/windowEvent.h"
#include "application/layers/layerStack.h"
#include "application/layers/imGuiLayer.h"

namespace daedalus {

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

	protected:
		Application(std::string title = "Daedalus Window", uint32_t width = 960, uint32_t height = 540, bool vsync = false);

	private:
		bool onWindowClose(event::WindowClosedEvent& e);
		bool onWindowResize(event::WindowResizedEvent& e);

	private:
		static Application* s_instance;
		
		Uni_ptr<application::Window> m_window;
		application::ImGuiLayer* m_ImGuiLayer;
		bool m_running = true;
		bool m_minimized = false;
		application::LayerStack m_layerStack;

		float m_lastFrameTime = 0.0f;
		application::Time m_time;
	};

	//To be defined in client
	Application* createApplication();

}