#include "2DTestLayer.h"

#include "imgui.h"

Layer2D::Layer2D()
	: m_camController(1280.0f / 720.0f)
{
}

void Layer2D::attach()
{
	DD_PROFILE_FUNCTION();

	m_texture = daedalusCore::graphics::Texture2D::create("resources/DD_testImage.png");
	m_texture2 = daedalusCore::graphics::Texture2D::create("resources/testImage.png");
	m_fullSubTextureTesting = daedalusCore::graphics::Texture2D::create("resources/subTextureTesting.png");
	m_subTexture = daedalusCore::graphics::SubTexture2D::createFromCoords(m_fullSubTextureTesting, { 24, 12 }, { 110, 240 });
	m_subTexture2 = daedalusCore::graphics::SubTexture2D::createFromCoords(m_fullSubTextureTesting, { 300, 16 }, {194, 213});
}

void Layer2D::detach()
{
	DD_PROFILE_FUNCTION();
}

void Layer2D::update(const daedalusCore::application::DeltaTime& dt)
{
	DD_PROFILE_FUNCTION();

	m_camController.update(dt);

#ifndef DD_DISTRO
	daedalusCore::graphics::Renderer2D::resetStats();
#endif
	{
		DD_PROFILE_SCOPE("renderer prep");
		daedalusCore::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		daedalusCore::graphics::RenderCommands::clear();
	}

	{
		DD_PROFILE_SCOPE("renderer draw");
		
		static float rotation = 0.0f;
		rotation += dt * 5.0f;

		daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.05f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.05f)
			{
				daedalusCore::maths::vec4 colour = { (x + 5.0f) / 10.0f , 0.2f, (y + 5.0f) / 10.0f, 0.5f };
				daedalusCore::graphics::Renderer2D::drawQuad({ {x, y, -0.1f}, { 0.2f }, colour });
			}
		}

		daedalusCore::graphics::Renderer2D::drawRotatedQuad(m_squareProps);
		daedalusCore::graphics::Renderer2D::drawQuad({ { -2, 0 }, { 0.75f, 0.75f }, { 0.3f, 0.8f, 0.2f, 1.0f } });
		daedalusCore::graphics::Renderer2D::drawQuad({ { 2, 0 }, { 0.5f, 0.5f }, m_texture });
		daedalusCore::graphics::Renderer2D::drawQuad({ { 2, 2 }, { 1.5f, 1.0f }, m_texture2 });
		daedalusCore::graphics::Renderer2D::drawRotatedQuad({{0, 1},{0.25f, 0.25f}, rotation, m_texture2, {1.0f, 0.6f, 0.6f, 1.0f}});
		daedalusCore::graphics::Renderer2D::drawQuad({ {-1, 0}, {0.5f, 0.5f}, m_subTexture });
		daedalusCore::graphics::Renderer2D::drawQuad({ {-1, -0.5f }, {0.5f, 0.5f}, m_subTexture2 });

		daedalusCore::graphics::Renderer2D::end();
	}
}

void Layer2D::imGuiRender()
{
	DD_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
#ifndef DD_DISTRO
	auto stats = daedalusCore::graphics::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());

	ImGui::NewLine();
#endif

	ImGui::ColorEdit4("Colour", &(m_squareProps.colour.x));
	ImGui::InputFloat3("position", &(m_squareProps.position.x));
	ImGui::InputFloat2("scale", &(m_squareProps.size.x));

	ImGui::SliderAngle("Z Rotation", &m_squareProps.rotation);

	ImGui::End();
}

void Layer2D::onEvent(daedalusCore::event::Event& e)
{
	m_camController.onEvent(e);
}