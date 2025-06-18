#include "jumperLayer.h"
#include <imgui.h>

#include "randomNumber.h"

namespace jumper {

	JumperLayer::JumperLayer()
		: m_gameCamera(1280.0f / 720.0f, 0.5f, true), m_levelManager(m_gameCamera)
	{
		RandomNumber::init();
	}

	void JumperLayer::attach()
	{
	}

	void JumperLayer::detach()
	{
	}

	void JumperLayer::update(const daedalusCore::application::DeltaTime& dt)
	{
#ifndef DD_DISTRO
		daedalusCore::graphics::Renderer2D::resetStats();
#endif

		// Clear
		daedalusCore::graphics::RenderCommands::setClearColour({ 0.2f, 0.2f, 0.2f, 1.0f });
		daedalusCore::graphics::RenderCommands::clear();

		// Update Logic
		if (m_jumperCharacter.update(dt))
			DD_LOG_INFO("Game end");

		m_gameCamera.update(dt, m_jumperCharacter);
		m_levelManager.update(m_jumperCharacter, m_gameCamera.getPosition());

		// Render
		daedalusCore::graphics::Renderer2D::begin(m_gameCamera.getCamera());
		m_levelManager.renderLevel();
		m_jumperCharacter.render();
		daedalusCore::graphics::Renderer2D::end();
	}

	void JumperLayer::imGuiRender()
	{
#ifndef DD_DISTRO
		ImGui::Begin("Renderer Stats");
		auto stats = daedalusCore::graphics::Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());
		ImGui::End();
#endif
		uint32_t playerScore = m_jumperCharacter.getScore();
		std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(ImGui::GetWindowPos(), 0xfffffffff, scoreStr.c_str());
	}

	void JumperLayer::onEvent(daedalusCore::event::Event& e)
	{
		m_gameCamera.onEvent(e);
	}


}