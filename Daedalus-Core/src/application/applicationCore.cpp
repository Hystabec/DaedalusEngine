#include "ddpch.h"
#include "applicationCore.h"

#include "application/window.h"
#include "application/input/Input.h"
#include "utils/timer.h"

#include "graphics/renderer/renderer.h"

namespace daedalusCore {

	Application* Application::s_instance = nullptr;

	Application::~Application()
	{
		graphics::Renderer::shutdown();
	}

	void Application::onEvent(event::Event& e)
	{
		event::EventDispatcher dispatch(e);
		dispatch.dispatch<event::WindowClosedEvent>(DD_BIND_EVENT_FUN(Application::onWindowClose));
		dispatch.dispatch<event::WindowResizedEvent>(DD_BIND_EVENT_FUN(Application::onWindowResize));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->onEvent(e);
			if (e.handled())
				break;
		}
	}

	void Application::init()
	{
		//Engine start Logic
	}

	int Application::run()
	{
		while (m_running)
		{
			application::DeltaTime dt = m_time.getDeltaTime();
			m_time.update();

			if (!m_minimized)
			{
				for (application::Layer* layer : m_layerStack)
					layer->update(dt);
			}

			m_ImGuiLayer->begin();
			for (application::Layer* layer : m_layerStack)
				layer->imGuiRender();
			m_ImGuiLayer->end();

			m_window->update();
		}

		return 0;
	}

	void Application::pushLayer(application::Layer* layer)
	{
		m_layerStack.pushLayer(layer);
		layer->attach();
	}

	void Application::pushOverlay(application::Layer* layer)
	{
		m_layerStack.pushOverlay(layer);
		layer->attach();
	}

	Application::Application(std::string title, unsigned int width, unsigned int height, bool vsync)
	{
		DD_CORE_ASSERT(!s_instance, "Duplicate application");
		s_instance = this;

		debug::Log::init();
		m_window = uni_ptr<application::Window>(application::Window::Create(application::WindowProperties(title, width, height, vsync)));
		m_window->setEventCallback(DD_BIND_EVENT_FUN(Application::onEvent));

		graphics::Renderer::init();

		m_ImGuiLayer = new application::ImGuiLayer;
		pushOverlay(m_ImGuiLayer);
	}

	bool Application::onWindowClose(event::WindowClosedEvent& e)
	{
		m_running = false;
		return true;
	}

	bool Application::onWindowResize(event::WindowResizedEvent& e)
	{
		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			m_minimized = true;
			return false;
		}

		m_minimized = false;
		graphics::Renderer::onWindowResize(e.getWidth(), e.getHeight());

		return false;
	}

}