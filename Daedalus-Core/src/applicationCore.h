#pragma once

#include "core.h"
#include "application/window.h"

#include "events/windowEvent.h"
#include "application/layers/layerStack.h"
#include "application/layers/imGuiLayer.h"

#include "graphics/shader.h"
#include "graphics/buffers/vertexArray.h"
#include "graphics/buffers/bufferLayout.h"
#include "graphics/buffers/indexBuffer.h"
#include "graphics/buffers/vertexBuffer.h"

namespace daedalusCore {

	class  Application
	{
	private:
		static Application* m_instance;

		std::unique_ptr<application::Window> m_window;
		application::ImGuiLayer* m_ImGuiLayer;
		bool m_running = true;
		application::layerStack m_layerStack;

		std::shared_ptr<graphics::Shader> m_shader;
		std::shared_ptr<graphics::buffers::VertexArray> m_vertexArray;
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