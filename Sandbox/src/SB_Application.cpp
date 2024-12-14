#include <Daedalus.h>

class TestLayer : public daedalusCore::application::Layer
{
private:
	daedalusCore::utils::Timer time;
	int frames = 0;

public:
	TestLayer()
	{
	}

	void Update() override
	{
		frames++;

		if (time.elapsedSeconds() >= 1.0f)
		{
			time.reset();
			LOG_INFO("FPS: {}", frames);
			frames = 0;
		}
	}

	void Event(daedalusCore::event::Event& e) override
	{
		
	}
};

class SandBox : public daedalusCore::Application
{
public:
	SandBox()
		: Application("SandBox", 960, 540, true)
	{
		this->PushLayer(new TestLayer());
	}

	~SandBox()
	{
	}

	/*void Tick() override
	{
		LOG_INFO("FPS {}", GetFramesPerSecond());
	}*/
};

daedalusCore::Application* daedalusCore::CreateApplication()
{
	return new SandBox();
}