#include "ddpch.h"
#include "applicationCore.h"

#include "application/window.h"
#include "utils/timer.h"

namespace daedalusCore {

	bool Application::OnWindowClose(event::WindowClosedEvent& e)
	{
		m_running = false;
		return true;
	}
	Application::Application(std::string title, unsigned int width, unsigned int height, bool vsync)
	{
		debug::Log::Init();
		m_window = std::unique_ptr<application::Window>(application::Window::Create(application::WindowProperties(title, width, height, vsync)));
		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(event::Event& e)
	{
		event::EventDispatcher dispatch(e);
		dispatch.Dispatch<event::WindowClosedEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->Event(e);
			if (e.Handled())
				break;
		}
	}

	void Application::Init()
	{
		//Engine start Logic
	}

	int Application::Run()
	{
		while (m_running)
		{
			for (application::Layer* layer : m_layerStack)
				layer->Update();

			m_window->Update();
		}

		return 0;
	}

	void Application::PushLayer(application::Layer* layer)
	{
		m_layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(application::Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
	}

}