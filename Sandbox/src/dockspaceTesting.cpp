#include "dockspaceTesting.h"
#include <imgui.h>

EditorLayer::EditorLayer()
	: m_camController(1280.0f / 720.0f)
{
}

void EditorLayer::attach()
{
	DD_PROFILE_FUNCTION();
	m_texture = daedalus::graphics::Texture2D::create("resources/testImage.png");

	daedalus::graphics::FramebufferSpecification fbSpec;
	fbSpec.width = 1280;
	fbSpec.height = 720;
	m_framebuffer = daedalus::graphics::Framebuffer::create(fbSpec);
}

void EditorLayer::detach()
{
	DD_PROFILE_FUNCTION();
}

void EditorLayer::update(const daedalus::application::DeltaTime& dt)
{
	DD_PROFILE_FUNCTION();
	m_camController.update(dt);

	daedalus::graphics::Renderer2D::resetStats();

	{
		DD_PROFILE_SCOPE("renderer prep");
		m_framebuffer->bind();
		daedalus::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		daedalus::graphics::RenderCommands::clear();
	}

	{
		DD_PROFILE_SCOPE("renderer draw");

		daedalus::graphics::Renderer2D::begin(m_camController.getCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.05f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.05f)
			{
				daedalus::maths::Vec4 colour = { (x + 5.0f) / 10.0f , 0.2f, (y + 5.0f) / 10.0f, 0.5f };
				daedalus::graphics::Renderer2D::drawQuad({ {x, y, -0.1f}, { 0.2f }, colour });
			}
		}

		daedalus::graphics::Renderer2D::drawQuad({ daedalus::maths::Vec2{ 2, 0 }, { 0.5f, 0.5f }, m_texture });

		daedalus::graphics::Renderer2D::end();
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
				daedalus::Application::get().close();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Settings");
	auto stats = daedalus::graphics::Renderer2D::getStats();
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

void EditorLayer::onEvent(daedalus::event::Event& e)
{
	DD_PROFILE_FUNCTION();
	m_camController.onEvent(e);
}
