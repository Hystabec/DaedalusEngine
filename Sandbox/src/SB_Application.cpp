#include <Daedalus.h>
#include "imgui.h"

class TestLayer : public daedalusCore::application::Layer
{
public:
	TestLayer()
		: m_othoCam(-1.6f, 1.6f, -0.9f, 0.9f), m_triPos(0)
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

		m_squareVertexArray.reset(graphics::buffers::VertexArray::Create());
		float sqrVerts[4 * 7] = {
			-0.75f, 0.75f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f,
			0.75f, 0.75f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f,
			0.75f, -0.75f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f,
			-0.75f, -0.75f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<graphics::buffers::VertexBuffer> sqrVertBuff(graphics::buffers::VertexBuffer::create(sqrVerts, sizeof(sqrVerts)));
		sqrVertBuff->setLayout(layout);
		m_squareVertexArray->addVertexBuffer(sqrVertBuff);
		uint32_t sqrIndices[3 * 2] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<graphics::buffers::IndexBuffer> sqrIndexBuff(graphics::buffers::IndexBuffer::create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t)));
		m_squareVertexArray->setIndexBuffer(sqrIndexBuff);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_colour;
	
			uniform mat4 u_projView;
			uniform mat4 u_transform;

			out vec3 v_pos;
			out vec4 v_col;

			void main()
			{
				v_pos = a_position;
				v_col = a_colour;
				gl_Position = u_projView * u_transform * vec4(a_position, 1.0);
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

		m_flatShader.reset(graphics::Shader::create("resources/flatVertex.vert", "resources/flatFrag.frag", true));
	}

	void update(const daedalusCore::application::DeltaTime& dt) override
	{
		daedalusCore::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		daedalusCore::graphics::RenderCommands::clear();

		if (daedalusCore::application::Input::getKeyDown(DD_INPUT_KEY_W))
			m_othoCam.setPosition(m_othoCam.getPosition() + (daedalusCore::maths::vec3(0, 1.0f, 0) * (float)dt));
		if (daedalusCore::application::Input::getKeyDown(DD_INPUT_KEY_D))
			m_othoCam.setPosition(m_othoCam.getPosition() + (daedalusCore::maths::vec3(1.0f, 0, 0) * (float)dt));
		if (daedalusCore::application::Input::getKeyDown(DD_INPUT_KEY_S))
			m_othoCam.setPosition(m_othoCam.getPosition() + (daedalusCore::maths::vec3(0, -1.0f, 0) * (float)dt));
		if (daedalusCore::application::Input::getKeyDown(DD_INPUT_KEY_A))
			m_othoCam.setPosition(m_othoCam.getPosition() + (daedalusCore::maths::vec3(-1.0f, 0, 0) * (float)dt));

		if (daedalusCore::application::Input::getKeyDown(DD_INPUT_KEY_I))
			m_triPos += daedalusCore::maths::vec3(0.0f, 1.0f, 0.0f) * (float)dt;
		if (daedalusCore::application::Input::getKeyDown(DD_INPUT_KEY_L))
			m_triPos += daedalusCore::maths::vec3(1.0f, 0.0f, 0.0f) * (float)dt;
		if (daedalusCore::application::Input::getKeyDown(DD_INPUT_KEY_K))
			m_triPos += daedalusCore::maths::vec3(0.0f, -1.0f, 0.0f) * (float)dt;
		if (daedalusCore::application::Input::getKeyDown(DD_INPUT_KEY_J))
			m_triPos += daedalusCore::maths::vec3(-1.0f, 0.0f, 0.0f) * (float)dt;

		daedalusCore::maths::mat4 triangleTransform = daedalusCore::maths::mat4::translate(m_triPos) * daedalusCore::maths::mat4::rotate(0.0f, {0,0,1}) * daedalusCore::maths::mat4::scale({ 0.1f });

		daedalusCore::maths::vec4 redCol(0.8f, 0.2f, 0.3f, 1.0f);
		daedalusCore::maths::vec4 blueCol(0.2f, 0.3f, 0.8f, 1.0f);

		daedalusCore::graphics::Renderer::begin(m_othoCam);

		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				daedalusCore::maths::mat4 squareTransform = daedalusCore::maths::mat4::translate(daedalusCore::maths::vec3(x * 0.2f, y * 0.2f, 0.0f)) * daedalusCore::maths::mat4::scale({0.1f});

				x % 2 == 0 ? m_flatShader->setUniform4f(redCol, "u_colour") : m_flatShader->setUniform4f(blueCol, "u_colour");

				daedalusCore::graphics::Renderer::submit(m_squareVertexArray, m_flatShader, squareTransform);
			}
		}

		daedalusCore::graphics::Renderer::submit(m_vertexArray, m_shader, triangleTransform);

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
		ImGui::Begin("Camera Control");
		daedalusCore::maths::vec3 camPos = m_othoCam.getPosition();
		float pos[3] = { camPos.x, camPos.y, camPos.z };
		if (ImGui::InputFloat3("Position", (float*)pos, "%.1f"))
			m_othoCam.setPosition({ pos[0], pos[1], pos[2] });

		float zRot = daedalusCore::maths::degrees_to_radians(m_othoCam.getRotation());
		if (ImGui::SliderAngle("Z Rotation", &zRot))
			m_othoCam.setRotation(daedalusCore::maths::radians_to_degrees(zRot));
		
		ImGui::End();
	}

	void onEvent(daedalusCore::event::Event& e) override
	{

	}

private:
	daedalusCore::utils::Timer time;
	int frames = 0;

	std::shared_ptr<daedalusCore::graphics::Shader> m_shader;
	std::shared_ptr<daedalusCore::graphics::Shader> m_flatShader;
	std::shared_ptr<daedalusCore::graphics::buffers::VertexArray> m_vertexArray;
	std::shared_ptr<daedalusCore::graphics::buffers::VertexArray> m_squareVertexArray;
	daedalusCore::graphics::OrthographicCamera m_othoCam;

	daedalusCore::maths::vec3 m_triPos;
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