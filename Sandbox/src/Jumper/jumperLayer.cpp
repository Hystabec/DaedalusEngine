#include "jumperLayer.h"

namespace jumper {

	JumperLayer::JumperLayer()
		: m_camController(1280.0f / 720.0f), m_testPlatform(daedalusCore::graphics::Texture2D::create("resources/jumperAssets/platfromTexture.png"))
	{
	}

	void JumperLayer::attach()
	{
	}

	void JumperLayer::detach()
	{
	}

	void JumperLayer::update(const daedalusCore::application::DeltaTime& dt)
	{
		// Clear
		daedalusCore::graphics::RenderCommands::setClearColour({ 0.2f, 0.2f, 0.2f, 1.0f });
		daedalusCore::graphics::RenderCommands::clear();

		// Update Logic
		m_jumperCharacter.update(dt);

		//m_camController.setPosition(m_jumperCharacter.getPosition());

		// Render
		daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());
		m_testPlatform.render();
		m_jumperCharacter.render();
		daedalusCore::graphics::Renderer2D::end();
	}

	void JumperLayer::imGuiRender()
	{
	}

	void JumperLayer::onEvent(daedalusCore::event::Event& e)
	{
		m_camController.onEvent(e);
	}

}