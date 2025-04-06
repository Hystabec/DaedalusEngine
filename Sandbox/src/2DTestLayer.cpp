#include "2DTestLayer.h"

#include "imgui.h"

Layer2D::Layer2D()
	: m_camController(1280.0f / 720.0f)
{
}

void Layer2D::attach()
{
	m_vertexArray = daedalusCore::graphics::buffers::VertexArray::Create();
	float sqrVerts[4 * 7] = {
		-0.75f, 0.75f, 0.0f,
		0.75f, 0.75f, 0.0f,
		0.75f, -0.75f, 0.0f,
		-0.75f, -0.75f, 0.0f
	};

	daedalusCore::graphics::buffers::BufferLayout sqrLayout =
	{
		{ DD_BUFFERS_VEC3, "a_position" }
	};

	daedalusCore::shr_ptr<daedalusCore::graphics::buffers::VertexBuffer> sqrVertBuff(daedalusCore::graphics::buffers::VertexBuffer::create(sqrVerts, sizeof(sqrVerts)));
	sqrVertBuff->setLayout(sqrLayout);
	m_vertexArray->addVertexBuffer(sqrVertBuff);
	uint32_t sqrIndices[3 * 2] = { 0, 1, 2, 2, 3, 0 };
	daedalusCore::shr_ptr<daedalusCore::graphics::buffers::IndexBuffer> sqrIndexBuff(daedalusCore::graphics::buffers::IndexBuffer::create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t)));
	m_vertexArray->setIndexBuffer(sqrIndexBuff);

	m_shader = daedalusCore::graphics::Shader::create("resources/shaders/flatShader.glsl");
	m_colour = daedalusCore::maths::vec4(0.8f, 0.2f, 0.3f, 1.0f);
}

void Layer2D::detach()
{
}

void Layer2D::update(const daedalusCore::application::DeltaTime& dt)
{
	m_camController.update(dt);

	daedalusCore::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
	daedalusCore::graphics::RenderCommands::clear();

	daedalusCore::graphics::Renderer::begin(m_camController.getCamera());

	m_shader->enable();
	m_shader->setUniform4f(m_colour, "u_colour");
	daedalusCore::graphics::Renderer::submit(m_vertexArray, m_shader);

	daedalusCore::graphics::Renderer::end();
}

void Layer2D::imGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Colour", &(m_colour.x));
	ImGui::End();
}

void Layer2D::onEvent(daedalusCore::event::Event& e)
{
	m_camController.onEvent(e);
}