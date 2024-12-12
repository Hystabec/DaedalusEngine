#include "ddpch.h"
#include "coreApplication.h"

#include "graphics/window.h"
#include "utils/timer.h"

namespace daedalusCore {

	bool Application::OnWindowClose(event::WindowClosedEvent& e)
	{
		m_running = false;
		return true;
	}
	Application::Application(std::string title, unsigned int width, unsigned int height, bool vsync)
	{
		m_window = std::unique_ptr<graphics::Window>(graphics::Window::Create(graphics::WindowProperties(title, width, height, vsync)));
		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{
		Close();
	}

	void Application::OnEvent(event::Event& e)
	{
		event::EventDispatcher dispatch(e);
		dispatch.Dispatch<event::WindowClosedEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
	}

	void Application::Init()
	{
		//Engine start Logic
		debug::Log::Init();

		//Application Start override function
		Start();
	}

	int Application::Run()
	{
		utils::Timer time;

		float timer = 0.0f;
		float updateTimer = 0.0f;
		float updateTick = 1.0f / 60.0f;
		unsigned int frames = 0;
		unsigned int updates = 0;

		while (m_running)
		{
			if (time.elapsedSeconds() - updateTimer > updateTick)
			{
				updates++;
				Update();
				updateTimer += updateTick;
			}

			frames++;
			Render();
			m_window->Update();

			if (time.elapsedSeconds() - timer > 1.0f)
			{
				timer += 1.0f;
				m_FramesPerSecond = frames;
				m_UpdatesPerSecond = updates;
				frames = 0;
				updates = 0;
				Tick();
			}
		}

		return 0;
	}

}