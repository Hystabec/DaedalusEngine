#pragma once

#include "core.h"
#include "application/window.h"

#include "application/time/time.h"

#include "events/windowEvent.h"
#include "application/layers/layerStack.h"
#include "application/layers/imGuiLayer.h"

namespace daedalusCore {

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

	protected:
		Application(std::string title = "Daedalus Window", unsigned int width = 960, unsigned int height = 540, bool vsync = false);

	private:
		bool onWindowClose(event::WindowClosedEvent& e);

	private:
		static Application* s_instance;

		uni_ptr<application::Window> m_window;
		application::ImGuiLayer* m_ImGuiLayer;
		bool m_running = true;
		application::LayerStack m_layerStack;

		float m_lastFrameTime = 0.0f;
		application::Time m_time;
	};

	//To be defined in client
	Application* createApplication();

}