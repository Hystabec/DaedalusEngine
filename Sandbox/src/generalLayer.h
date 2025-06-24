#pragma once

#include <Daedalus.h>
#include "imgui.h"

class generalLayer : public daedalus::application::Layer
{
public:
	generalLayer()
		: m_camController(1280.0f / 720.0f), m_triPos(0)
	{
		using namespace daedalus;

		//triangle
		m_vertexArray = graphics::buffers::VertexArray::Create();
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

		m_squareVertexArray = graphics::buffers::VertexArray::Create();
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

		std::string triangleVertexSrc = R"(
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

		std::string triangleFragSrc = R"(
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

		auto triShader = graphics::Shader::create("TriangleShader", triangleVertexSrc, triangleFragSrc);
		m_shaderLib.add("TriangleShader", triShader);

		m_shaderLib.load("resources/shaders/flatShader.glsl");

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

		m_texuterVerexArray = graphics::buffers::VertexArray::Create();
		shr_ptr<graphics::buffers::VertexBuffer> textureVerBuff(graphics::buffers::VertexBuffer::create(textureVerts, sizeof(textureVerts)));
		textureVerBuff->setLayout(textureLayout);
		m_texuterVerexArray->addVertexBuffer(textureVerBuff);
		shr_ptr<graphics::buffers::IndexBuffer> textureIndexBuff(graphics::buffers::IndexBuffer::create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t)));
		m_texuterVerexArray->setIndexBuffer(textureIndexBuff);

		m_shaderLib.load("texSha", "resources/shaders/textureShader.glsl");

		m_texture = daedalus::graphics::Texture2D::create("resources/testImage.png");
		m_DDTestImage = daedalus::graphics::Texture2D::create("resources/DD_testImage.png");

		m_shaderLib.get("texSha")->setUniform1i(0, "u_texture");
	}

	void update(const daedalus::application::DeltaTime& dt) override
	{
		m_camController.update(dt);

		daedalus::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		daedalus::graphics::RenderCommands::clear();

		if (daedalus::application::Input::getKeyDown(DD_INPUT_KEY_I))
			m_triPos += daedalus::maths::vec3(0.0f, 1.0f, 0.0f) * (float)dt;
		if (daedalus::application::Input::getKeyDown(DD_INPUT_KEY_L))
			m_triPos += daedalus::maths::vec3(1.0f, 0.0f, 0.0f) * (float)dt;
		if (daedalus::application::Input::getKeyDown(DD_INPUT_KEY_K))
			m_triPos += daedalus::maths::vec3(0.0f, -1.0f, 0.0f) * (float)dt;
		if (daedalus::application::Input::getKeyDown(DD_INPUT_KEY_J))
			m_triPos += daedalus::maths::vec3(-1.0f, 0.0f, 0.0f) * (float)dt;

		daedalus::maths::mat4 triangleTransform = daedalus::maths::mat4::translate(m_triPos) * daedalus::maths::mat4::rotate(0.0f, { 0,0,1 }) * daedalus::maths::mat4::scale({ 0.1f });

		daedalus::maths::vec4 redCol(0.8f, 0.2f, 0.3f, 1.0f);
		daedalus::maths::vec4 greenCol(0.2f, 0.8f, 0.3f, 1.0f);
		daedalus::maths::vec4 blueCol(0.2f, 0.3f, 0.8f, 1.0f);

		daedalus::graphics::Renderer::begin(m_camController.getCamera());

		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				daedalus::maths::mat4 squareTransform = daedalus::maths::mat4::translate(daedalus::maths::vec3(x * 0.2f, y * 0.2f, 0.0f)) * daedalus::maths::mat4::scale({ 0.1f });

				auto flatShader = m_shaderLib.get("flatShader");
				flatShader->enable();
				x % 2 == 0 ? flatShader->setUniform4f(redCol, "u_colour") : flatShader->setUniform4f(greenCol, "u_colour");

				daedalus::graphics::Renderer::submit(m_squareVertexArray, flatShader, squareTransform);
			}
		}

		auto textureShader = m_shaderLib.get("texSha");
		m_texture->bind();
		daedalus::graphics::Renderer::submit(m_texuterVerexArray, textureShader, daedalus::maths::mat4::translate(daedalus::maths::vec3(-1, 0, 0)) * daedalus::maths::mat4::scale({ 1.5f }));
		m_DDTestImage->bind();
		daedalus::graphics::Renderer::submit(m_texuterVerexArray, textureShader, daedalus::maths::mat4::translate(daedalus::maths::vec3(-1, 0, 0)) * daedalus::maths::mat4::scale({ 1.5f }));

		daedalus::graphics::Renderer::submit(m_vertexArray, m_shaderLib.get("TriangleShader"), triangleTransform);

		daedalus::graphics::Renderer::end();

		frames++;

		if (time.elapsedSeconds() >= 1.0f)
		{
			time.reset();
			DD_LOG_INFO("FPS: {}", frames);
			frames = 0;
		}
	}

	void imGuiRender() override
	{
		ImGui::Begin("Camera Control");
		daedalus::maths::vec3 camPos = m_camController.getPosition();
		float pos[3] = { camPos.x, camPos.y, camPos.z };
		if (ImGui::InputFloat3("Position", (float*)pos, "%.1f"))
			m_camController.setPosition({ pos[0], pos[1], pos[2] });

		float zRot = daedalus::maths::degrees_to_radians(m_camController.getRotation());
		if (ImGui::SliderAngle("Z Rotation", &zRot))
			m_camController.setRotation(daedalus::maths::radians_to_degrees(zRot));

		ImGui::End();
	}

	void onEvent(daedalus::event::Event& e) override
	{
		m_camController.onEvent(e);
	}

private:
	daedalus::utils::Timer time;
	int frames = 0;

	daedalus::graphics::ShaderLibrary m_shaderLib;

	daedalus::shr_ptr<daedalus::graphics::buffers::VertexArray> m_vertexArray, m_squareVertexArray, m_texuterVerexArray;
	daedalus::shr_ptr<daedalus::graphics::Texture2D> m_texture, m_DDTestImage;

	daedalus::graphics::OrthographicCameraController m_camController;

	daedalus::maths::vec3 m_triPos;
};