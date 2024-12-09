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
	}
};

daedalusCore::Application* daedalusCore::CreateApplication()
{
	return new SandBox();
}