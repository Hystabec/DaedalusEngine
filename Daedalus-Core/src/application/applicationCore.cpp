#include "ddpch.h"
#include "applicationCore.h"

#include "application/window.h"
#include "application/input/Input.h"
#include "utils/timer.h"

#include "graphics/renderer/renderer.h"

namespace daedalus {

	Application* Application::s_instance = nullptr;

	Application::~Application()
	{
		DD_PROFILE_FUNCTION();

		graphics::Renderer::shutdown();
	}

	void Application::onEvent(event::Event& e)
	{
		DD_PROFILE_FUNCTION();

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
		DD_PROFILE_FUNCTION();

		while (m_running)
		{
			DD_PROFILE_SCOPE("Loop of run");

			application::DeltaTime dt = m_time.getDeltaTime();
			m_time.update();

			if (!m_minimized)
			{
				{
					DD_PROFILE_SCOPE("update() | layers in layerstack");
					for (application::Layer* layer : m_layerStack)
						layer->update(dt);
				}
			}

			{
				DD_PROFILE_SCOPE("imGuiRenderer() | layers in layerstack");
				m_ImGuiLayer->begin();
				for (application::Layer* layer : m_layerStack)
					layer->imGuiRender();
				m_ImGuiLayer->end();
			}

			m_window->update();
		}

		return 0;
	}

	void Application::pushLayer(application::Layer* layer)
	{
		DD_PROFILE_FUNCTION();

		m_layerStack.pushLayer(layer);
		layer->attach();
	}

	void Application::pushOverlay(application::Layer* layer)
	{
		DD_PROFILE_FUNCTION();

		m_layerStack.pushOverlay(layer);
		layer->attach();
	}

	void Application::close()
	{
		m_running = false;
	}

	Application::Application(std::string title, uint32_t width, uint32_t height, bool vsync)
	{
		DD_PROFILE_FUNCTION();

		DD_CORE_ASSERT(!s_instance, "Duplicate application");
		s_instance = this;

		debug::Log::init();
		m_window = Uni_ptr<application::Window>(application::Window::Create(application::WindowProperties(title, width, height, vsync)));
		m_window->setEventCallback(DD_BIND_EVENT_FUN(Application::onEvent));

		graphics::Renderer::init();

		m_ImGuiLayer = new application::ImGuiLayer;
		pushOverlay(m_ImGuiLayer);
	}

	bool Application::onWindowClose(event::WindowClosedEvent& e)
	{
		close();
		return true;
	}

	bool Application::onWindowResize(event::WindowResizedEvent& e)
	{
		DD_PROFILE_FUNCTION();

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