#include <Daedalus.h>
#include "imgui.h"

class TestLayer : public daedalusCore::application::Layer
{
public:
	TestLayer()
		: m_othoCam(-1.6f, 1.6f, -0.9f, 0.9f), m_triPos(0)
	{
		using namespace daedalusCore;
		//triangle
		m_vertexArray.reset(graphics::buffers::VertexArray::Create());
		float vertcies[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.5f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.4f, 0.7f, 0.0f, 1.0f
		};

		shr_ptr<graphics::buffers::VertexBuffer> verBuff(graphics::buffers::VertexBuffer::create(vertcies, sizeof(vertcies)));

		graphics::buffers::BufferLayout layout =
		{
			{ DD_BUFFERS_VEC3, "a_position" },
			{ DD_BUFFERS_VEC4, "a_colour" }
		};

		verBuff->setLayout(layout);
		m_vertexArray->addVertexBuffer(verBuff);

		uint32_t indices[3] = { 0, 1, 2 };
		shr_ptr<graphics::buffers::IndexBuffer> indexBuff(graphics::buffers::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuff);
		//triangle end

		m_squareVertexArray.reset(graphics::buffers::VertexArray::Create());
		float sqrVerts[4 * 7] = {
			-0.75f, 0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			-0.75f, -0.75f, 0.0f
		};

		graphics::buffers::BufferLayout sqrLayout =
		{
			{ DD_BUFFERS_VEC3, "a_position" }
		};

		shr_ptr<graphics::buffers::VertexBuffer> sqrVertBuff(graphics::buffers::VertexBuffer::create(sqrVerts, sizeof(sqrVerts)));
		sqrVertBuff->setLayout(sqrLayout);
		m_squareVertexArray->addVertexBuffer(sqrVertBuff);
		uint32_t sqrIndices[3 * 2] = { 0, 1, 2, 2, 3, 0 };
		shr_ptr<graphics::buffers::IndexBuffer> sqrIndexBuff(graphics::buffers::IndexBuffer::create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t)));
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

		float textureVerts[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		graphics::buffers::BufferLayout textureLayout = {
			{ DD_BUFFERS_VEC3, "a_position" },
			{ DD_BUFFERS_VEC2, "a_texCoord" }
		};

		m_texuterVerexArray.reset(graphics::buffers::VertexArray::Create());
		shr_ptr<graphics::buffers::VertexBuffer> textureVerBuff(graphics::buffers::VertexBuffer::create(textureVerts, sizeof(textureVerts)));
		textureVerBuff->setLayout(textureLayout);
		m_texuterVerexArray->addVertexBuffer(textureVerBuff);
		shr_ptr<graphics::buffers::IndexBuffer> textureIndexBuff(graphics::buffers::IndexBuffer::create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t)));
		m_texuterVerexArray->setIndexBuffer(textureIndexBuff);

		std::string texutureVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec2 a_texCoord;
	
			uniform mat4 u_projView;
			uniform mat4 u_transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_texCoord;
				gl_Position = u_projView * u_transform * vec4(a_position, 1.0);
			}
		)";

		std::string textureFragSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 colour;

			in vec2 v_TexCoord;

			uniform sampler2D u_texture;

			void main()
			{
				colour = texture(u_texture, v_TexCoord);
			}
		)";

		m_texutureShader.reset(daedalusCore::graphics::Shader::create(texutureVertexSrc.c_str(), textureFragSrc.c_str(), false));

		m_texture.reset(daedalusCore::graphics::Texture2D::Create("resources/testImage3.png"));
		m_texutureShader->setUniform1i(0, "u_texture");
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
		daedalusCore::maths::vec4 greenCol(0.2f, 0.8f, 0.3f, 1.0f);
		daedalusCore::maths::vec4 blueCol(0.2f, 0.3f, 0.8f, 1.0f);

		daedalusCore::graphics::Renderer::begin(m_othoCam);

		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				daedalusCore::maths::mat4 squareTransform = daedalusCore::maths::mat4::translate(daedalusCore::maths::vec3(x * 0.2f, y * 0.2f, 0.0f)) * daedalusCore::maths::mat4::scale({0.1f});

				m_flatShader->enable();
				x % 2 == 0 ? m_flatShader->setUniform4f(redCol, "u_colour") : m_flatShader->setUniform4f(greenCol, "u_colour");

				daedalusCore::graphics::Renderer::submit(m_squareVertexArray, m_flatShader, squareTransform);
			}
		}

		m_texture->bind();
		daedalusCore::graphics::Renderer::submit(m_texuterVerexArray, m_texutureShader, daedalusCore::maths::mat4::translate(daedalusCore::maths::vec3(-1, 0, 0)) * daedalusCore::maths::mat4::scale({1.5f}));

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

	daedalusCore::shr_ptr<daedalusCore::graphics::Shader> m_shader, m_flatShader, m_texutureShader;
	daedalusCore::shr_ptr<daedalusCore::graphics::buffers::VertexArray> m_vertexArray, m_squareVertexArray, m_texuterVerexArray;
	daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> m_texture;

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