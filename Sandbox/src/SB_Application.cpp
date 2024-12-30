#include <Daedalus.h>
#include "imgui.h"

#include <src/graphics/shader.h>
#include <src/graphics/layers/graphicsLayer.h>
#include <src/graphics/batchRenderer2D.h>
#include <src/graphics/sprite.h>
#include <src/utils/colourConversionUtils.h>

class TileLayer : public daedalusCore::graphics::Layer
{
public:
	TileLayer(daedalusCore::graphics::Shader* shader)
		: daedalusCore::graphics::Layer(new daedalusCore::graphics::BatchRenderer2D(), shader, daedalusCore::maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1, 1))
	{
	}
};

class TestLayer : public daedalusCore::application::Layer
{

private:
	daedalusCore::utils::Timer time;
	int frames = 0;

	//Temp graphics
	//daedalusCore::graphics::Shader* m_shader;
	//TileLayer* m_layer;

public:
	TestLayer()
	{
		//m_shader = daedalusCore::graphics::Shader::create("../Daedalus-Core/resources/shaders/basic.vert", "../Daedalus-Core/resources/shaders/basic.frag");
		//m_shader->enable();
		//m_layer = new TileLayer(m_shader);
		//m_layer->add(new daedalusCore::graphics::Sprite(0.0f, 0.0f, 1.0f, 1.0f, daedalusCore::utils::colour_vec4_to_uint(daedalusCore::maths::vec4(1.0f, 0.0f, 0.0f, 1.0f))));
	}

	~TestLayer()
	{
		//delete m_layer;
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

		//m_shader->enable();
		//m_layer->render();
		//m_shader->disable();
	}

	void ImGuiRender()
	{
		
	}

	void OnEvent(daedalusCore::event::Event& e) override
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
};

daedalusCore::Application* daedalusCore::CreateApplication()
{
	return new SandBox();
}