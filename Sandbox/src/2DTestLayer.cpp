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
}

void Layer2D::detach()
{
	DD_PROFILE_FUNCTION();
}

void Layer2D::update(const daedalusCore::application::DeltaTime& dt)
{
	DD_PROFILE_FUNCTION();

	m_camController.update(dt);

	{
		DD_PROFILE_SCOPE("renderer prep");
		daedalusCore::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		daedalusCore::graphics::RenderCommands::clear();
	}

	{
		DD_PROFILE_SCOPE("renderer draw");
		daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());

		daedalusCore::graphics::Renderer2D::drawRotatedQuad(m_squareProps);
		daedalusCore::graphics::Renderer2D::drawQuad({ { -2, 0 }, { 0.75f, 0.75f }, { 0.3f, 0.8f, 0.2f, 1.0f } });
		daedalusCore::graphics::Renderer2D::drawQuad({ { 2, 0 }, { 0.5f, 0.5f }, m_texture });

		daedalusCore::graphics::Renderer2D::end();
	}
}

void Layer2D::imGuiRender()
{
	DD_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Colour", &(m_squareProps.colour.x));
	ImGui::InputFloat3("position", &(m_squareProps.position.x));
	ImGui::InputFloat2("scale", &(m_squareProps.size.x));

	float asRads = daedalusCore::maths::degrees_to_radians(-m_squareProps.rotation);
	if (ImGui::SliderAngle("Z Rotation", &asRads))
		m_squareProps.rotation = daedalusCore::maths::radians_to_degrees(-asRads);

	ImGui::End();
}

void Layer2D::onEvent(daedalusCore::event::Event& e)
{
	m_camController.onEvent(e);
}