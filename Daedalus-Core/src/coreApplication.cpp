#include "ddpch.h"
#include "coreApplication.h"

#include "graphics/window.h"
#include "utils/timer.h"
#include "debugTools/log.h"

namespace daedalusCore {

	Application::Application(const char* applicationName)
	{
		m_applicationName = applicationName;
	}

	Application::~Application()
	{
		Close();
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
		graphics::Window window(m_applicationName, 960, 540);
		utils::Timer time;

		float timer = 0.0f;
		float updateTimer = 0.0f;
		float updateTick = 1.0f / 60.0f;
		unsigned int frames = 0;
		unsigned int updates = 0;

		while (!window.closed())
		{
			window.clear();

			if (time.elapsedSeconds() - updateTimer > updateTick)
			{
				updates++;
				Update();
				updateTimer += updateTick;
			}

			frames++;
			Render();
			window.update();

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