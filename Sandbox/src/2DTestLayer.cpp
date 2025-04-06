#include "2DTestLayer.h"

#include "imgui.h"

Layer2D::Layer2D()
	: m_camController(1280.0f / 720.0f)
{
}

void Layer2D::attach()
{
}

void Layer2D::detach()
{
}

void Layer2D::update(const daedalusCore::application::DeltaTime& dt)
{
	m_camController.update(dt);

	daedalusCore::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
	daedalusCore::graphics::RenderCommands::clear();

	daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());

	daedalusCore::graphics::Renderer2D::drawQuad(m_position, m_scale, m_zRot, m_colour);
	daedalusCore::graphics::Renderer2D::drawQuad({ -2, 0 }, { 0.75f, 0.75f }, { 0.3f, 0.8f, 0.2f, 1.0f });

	daedalusCore::graphics::Renderer2D::end();
}

void Layer2D::imGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Colour", &(m_colour.x));
	ImGui::InputFloat2("position", &(m_position.x));
	ImGui::InputFloat2("scale", &(m_scale.x));

	float asRads = daedalusCore::maths::degrees_to_radians(-m_zRot);
	if (ImGui::SliderAngle("Z Rotation", &asRads))
		m_zRot = daedalusCore::maths::radians_to_degrees(-asRads);

	ImGui::End();
}

void Layer2D::onEvent(daedalusCore::event::Event& e)
{
	m_camController.onEvent(e);
}