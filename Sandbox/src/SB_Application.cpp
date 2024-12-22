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
			DD_LOG_INFO("FPS: {}", frames);
			frames = 0;
		}
	}

	void OnEvent(daedalusCore::event::Event& e) override
	{
		using namespace daedalusCore::event;
		if (e.GetType() == EventType::KeyPressed)
		{
			KeyPressedEvent& event = (KeyPressedEvent&)e;
			DD_LOG_TRACE("Pressed code: {}", event.GetKeyCode());
		}
	}
};

class SandBox : public daedalusCore::Application
{
public:
	SandBox()
		: Application("SandBox", 960, 540, true)
	{
		this->PushLayer(new TestLayer());
		this->PushLayer(new daedalusCore::application::ImGuiLayer());
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