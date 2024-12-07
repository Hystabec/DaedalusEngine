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
		daedalusCore::debug::Log::TestLog("FPS {}", GetFramesPerSecond(), 6, "fd");
		daedalusCore::debug::Log::TestLog("Hello world");
	}
};

daedalusCore::Application* daedalusCore::CreateApplication()
{
	return new SandBox();
}