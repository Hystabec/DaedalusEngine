#include "ddpch.h"
#include "applicationCore.h"

#include "application/window.h"
#include "application/Input.h"
#include "utils/timer.h"

#include "graphics/renderer/renderer.h"

namespace daedalusCore {

	Application* Application::s_instance = nullptr;

	Application::~Application()
	{
	}

	void Application::onEvent(event::Event& e)
	{
		event::EventDispatcher dispatch(e);
		dispatch.dispatch<event::WindowClosedEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));

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
			graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
			graphics::RenderCommands::clear();

			for (application::Layer* layer : m_layerStack)
				layer->update();

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
		m_window = std::unique_ptr<application::Window>(application::Window::Create(application::WindowProperties(title, width, height, vsync)));
		m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		m_ImGuiLayer = new application::ImGuiLayer;
		pushOverlay(m_ImGuiLayer);
	}

	bool Application::onWindowClose(event::WindowClosedEvent& e)
	{
		m_running = false;
		return true;
	}

}