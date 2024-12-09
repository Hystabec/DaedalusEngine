#include <Daedalus.h>
#include <iostream>

class SandBox : public daedalusCore::Application
{
public:
	SandBox()
		: Application("Sandbox Application")
	{
	}

	~SandBox()
	{
	}

	void Tick() override
	{
		LOG_INFO("FPS {}", GetFramesPerSecond());
		LOG_TRACE("hello {}", 64);
		LOG_WARN("jello {}", 'I', 16);
		LOG_ERROR(true);
		LOG_CRITICAL("apple {} {}", 843.432f);
	}
};

daedalusCore::Application* daedalusCore::CreateApplication()
{
	return new SandBox();
}