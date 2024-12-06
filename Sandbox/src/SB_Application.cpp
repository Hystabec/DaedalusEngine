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
		daedalusCore::Log::GetCoreLogger()->info("FPS {}", GetFramesPerSecond());
	}
};

daedalusCore::Application* daedalusCore::CreateApplication()
{
	return new SandBox();
}