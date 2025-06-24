#include "EditorLayer.h"
#include <imgui.h>

EditorLayer::EditorLayer()
	: m_camController(1280.0f / 720.0f)
{
}

void EditorLayer::attach()
{
	DD_PROFILE_FUNCTION();

	daedalusCore::graphics::FramebufferSpecification fbSpec;
	fbSpec.width = 1280;
	fbSpec.height = 720;
	m_framebuffer = daedalusCore::graphics::Framebuffer::create(fbSpec);
}

void EditorLayer::detach()
{
	DD_PROFILE_FUNCTION();
}

void EditorLayer::update(const daedalusCore::application::DeltaTime& dt)
{
	DD_PROFILE_FUNCTION();
	m_camController.update(dt);

	daedalusCore::graphics::Renderer2D::resetStats();

	{
		DD_PROFILE_SCOPE("renderer prep");
		m_framebuffer->bind();
		daedalusCore::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		daedalusCore::graphics::RenderCommands::clear();
	}

	{
		DD_PROFILE_SCOPE("renderer draw");

		daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.05f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.05f)
			{
				daedalusCore::maths::vec4 colour = { (x + 5.0f) / 10.0f , 0.2f, (y + 5.0f) / 10.0f, 0.5f };
				daedalusCore::graphics::Renderer2D::drawQuad({ {x, y, -0.1f}, { 0.2f }, colour });
			}
		}

		daedalusCore::graphics::Renderer2D::end();
		m_framebuffer->unbind();
	}
}

void EditorLayer::imGuiRender()
{
	DD_PROFILE_FUNCTION();
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
				daedalusCore::Application::get().close();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Settings");
	auto stats = daedalusCore::graphics::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());
	ImGui::End();

	ImGui::Begin("Viewport");
	uint32_t textureID = m_framebuffer->getColourAttachmentRendererID();
	ImGui::Image(textureID, ImVec2{1280.0f, 720.0f});
	//ImGui::Image(textureID, ImGui::GetWindowSize());
	ImGui::End();
}

void EditorLayer::onEvent(daedalusCore::event::Event& e)
{
	DD_PROFILE_FUNCTION();
	m_camController.onEvent(e);
}
