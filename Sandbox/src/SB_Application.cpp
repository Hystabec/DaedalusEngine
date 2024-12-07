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
		//std::cout << GetFramesPerSecond() << " fps\n";
		//DD_CORE_LOG_TRACE("FPS {}", GetFramesPerSecond());
		//daedalusCore::debug::Log::Trace("FPS {}", GetFramesPerSecond());
		DD_CORE_LOG_INFO("FPS {}", GetFramesPerSecond());
		daedalusCore::debug::Log::Test_Trace("Hello world %d", 322);
	}
};

daedalusCore::Application* daedalusCore::CreateApplication()
{
	return new SandBox();
}