#include <Daedalus.h>
#include "imgui.h"

#include "graphics/shader.h"
#include "graphics/buffers/vertexArray.h"
#include "graphics/buffers/bufferLayout.h"
#include "graphics/buffers/indexBuffer.h"
#include "graphics/buffers/vertexBuffer.h"
#include "graphics/renderer/renderer.h"

class TestLayer : public daedalusCore::application::Layer
{

private:
	daedalusCore::utils::Timer time;
	int frames = 0;

	std::shared_ptr<daedalusCore::graphics::Shader> m_shader;
	std::shared_ptr<daedalusCore::graphics::buffers::VertexArray> m_vertexArray;
public:
	TestLayer()
	{
		using namespace daedalusCore;
		m_vertexArray.reset(graphics::buffers::VertexArray::Create());
		float vertcies[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.5f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.4f, 0.7f, 0.0f, 1.0f
		};

		std::shared_ptr<graphics::buffers::VertexBuffer> verBuff(graphics::buffers::VertexBuffer::create(vertcies, sizeof(vertcies)));

		graphics::buffers::BufferLayout layout =
		{
			{ DD_BUFFERS_VEC3, "a_position" },
			{ DD_BUFFERS_VEC4, "a_colour" }
		};

		verBuff->setLayout(layout);
		m_vertexArray->addVertexBuffer(verBuff);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<graphics::buffers::IndexBuffer> indexBuff(graphics::buffers::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuff);

		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_colour;
	
			out vec3 v_pos;
			out vec4 v_col;

			void main()
			{
				v_pos = a_position;
				v_col = a_colour;
				gl_Position = vec4(a_position, 1.0);
			}
		)";

		std::string fragSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 colour;
			in vec3 v_pos;
			in vec4 v_col;

			void main()
			{
				colour = vec4(v_pos * 0.5 + 0.5, 1.0);
				colour = v_col;
			}
		)";

		m_shader.reset(graphics::Shader::create(vertexSrc.c_str(), fragSrc.c_str(), false));
	}

	void update() override
	{
		daedalusCore::graphics::Renderer::begin();

		m_shader->enable();
		daedalusCore::graphics::Renderer::submit(m_vertexArray);

		daedalusCore::graphics::Renderer::end();

		frames++;

		if (time.elapsedSeconds() >= 1.0f)
		{
			time.reset();
			DD_LOG_INFO("FPS: {}", frames);
			frames = 0;
		}
	}

	void imGuiRender()
	{
		
	}

	void onEvent(daedalusCore::event::Event& e) override
	{

	}
};

class SandBox : public daedalusCore::Application
{
public:
	SandBox()
		: Application("SandBox", 960, 540, true)
	{
		this->pushLayer(new TestLayer());
	}

	~SandBox()
	{
	}
};

daedalusCore::Application* daedalusCore::createApplication()
{
	return new SandBox();
}