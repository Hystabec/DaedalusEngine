#include "2DTestLayer.h"

#include "imgui.h"

Layer2D::Layer2D()
	: m_camController(1280.0f / 720.0f)
{
}

void Layer2D::attach()
{
	DD_PROFILE_FUNCTION();

	//m_texture = daedalus::graphics::Texture2D::create("assets/DD_testImage.png");
	//m_texture2 = daedalus::graphics::Texture2D::create("assets/testImage.png");
	//m_fullSubTextureTesting = daedalus::graphics::Texture2D::create("assets/subTextureTesting.png");
	m_subTexture = daedalus::graphics::SubTexture2D::createFromCoords(m_fullSubTextureTesting, { 24, 12 }, { 110, 240 });
	m_subTexture2 = daedalus::graphics::SubTexture2D::createFromCoords(m_fullSubTextureTesting, { 300, 16 }, {194, 213});
}

void Layer2D::detach()
{
	DD_PROFILE_FUNCTION();
}

void Layer2D::update(const daedalus::application::DeltaTime& dt)
{
	DD_PROFILE_FUNCTION();

	m_camController.update(dt);

#ifndef DD_DISTRO
	daedalus::graphics::Renderer2D::resetStats();
#endif
	{
		DD_PROFILE_SCOPE("renderer prep");
		daedalus::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		daedalus::graphics::RenderCommands::clear();
	}

	{
		DD_PROFILE_SCOPE("renderer draw");
		
		static float rotation = 0.0f;
		rotation += dt * 5.0f;

		daedalus::graphics::Renderer2D::begin(m_camController.getCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.05f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.05f)
			{
				daedalus::maths::Vec4 colour = { (x + 5.0f) / 10.0f , 0.2f, (y + 5.0f) / 10.0f, 0.5f };
				daedalus::graphics::Renderer2D::drawQuad({ {x, y, -0.1f}, { 0.2f }, colour });
			}
		}

		daedalus::graphics::Renderer2D::drawRotatedQuad(m_squareProps);
		daedalus::graphics::Renderer2D::drawQuad({ daedalus::maths::Vec2{ -2, 0 }, { 0.75f, 0.75f }, { 0.3f, 0.8f, 0.2f, 1.0f } });
		//daedalus::graphics::Renderer2D::drawQuad({ daedalus::maths::Vec2{ 2, 0 }, { 0.5f, 0.5f }, m_texture });
		//daedalus::graphics::Renderer2D::drawQuad({ daedalus::maths::Vec2{ 2, 2 }, { 1.5f, 1.0f }, m_texture2 });
		//daedalus::graphics::Renderer2D::drawRotatedQuad({ daedalus::maths::Vec2{0, 1},{0.25f, 0.25f}, rotation, m_texture2, {1.0f, 0.6f, 0.6f, 1.0f}});
		daedalus::graphics::Renderer2D::drawQuad({ daedalus::maths::Vec2{-1, 0}, {0.5f, 0.5f}, m_subTexture });
		daedalus::graphics::Renderer2D::drawQuad({ daedalus::maths::Vec2{-1, -0.5f }, {0.5f, 0.5f}, m_subTexture2 });

		daedalus::graphics::Renderer2D::end();
	}
}

void Layer2D::imGuiRender()
{
	DD_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
#ifndef DD_DISTRO
	auto stats = daedalus::graphics::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());

	ImGui::NewLine();
#endif

	ImGui::ColorEdit4("Colour", &(m_squareProps.colour.x));
	//ImGui::InputFloat3("position", &(m_squareProps.position.x));
	//ImGui::InputFloat2("scale", &(m_squareProps.size.x));

	//ImGui::SliderAngle("Z Rotation", &m_squareProps.rotation);

	ImGui::End();
}

void Layer2D::onEvent(daedalus::event::Event& e)
{
	m_camController.onEvent(e);
}