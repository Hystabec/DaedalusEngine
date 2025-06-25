#include "EditorLayer.h"
#include <imgui.h>

namespace daedalus
{

	EditorLayer::EditorLayer()
		: m_camController(1280.0f / 720.0f)
	{
	}

	void EditorLayer::attach()
	{
		DD_PROFILE_FUNCTION();

		graphics::FramebufferSpecification fbSpec;
		fbSpec.width = 1280;
		fbSpec.height = 720;
		m_framebuffer = graphics::Framebuffer::create(fbSpec);

		m_activeScene = create_shr_ptr<scene::Scene>();
		auto square = m_activeScene->createEntity("Square");
		square.addComponent<scene::SpriteRendererComponent>(maths::vec4{ 0.8f, 0.2f, 0.2f, 1.0f });
	}

	void EditorLayer::detach()
	{
		DD_PROFILE_FUNCTION();
	}

	void EditorLayer::update(const application::DeltaTime& dt)
	{
		DD_PROFILE_FUNCTION();

		if(m_viewportFocused)
			m_camController.update(dt);

		

		graphics::Renderer2D::resetStats();

		m_framebuffer->bind();
		graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		graphics::RenderCommands::clear();


		graphics::Renderer2D::begin(m_camController.getCamera());

		// update scene
		m_activeScene->update(dt);

#if 0
			for (float y = -5.0f; y < 5.0f; y += 0.05f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.05f)
				{
					maths::vec4 colour = { (x + 5.0f) / 10.0f , 0.2f, (y + 5.0f) / 10.0f, 0.5f };
					graphics::Renderer2D::drawQuad({ {x, y, -0.1f}, { 0.2f }, colour });
				}
			}
#endif

		graphics::Renderer2D::end();
		m_framebuffer->unbind();
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
					Application::get().close();

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Settings");
		auto stats = graphics::Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::get().getImGuiLayer()->setAllowEvents(m_viewportFocused && m_viewportHovered);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		daedalus::maths::vec2 viewportSizeAsVec2 = { viewportSize.x, viewportSize.y };
		if (m_viewportSize != viewportSizeAsVec2)
		{
			m_viewportSize = viewportSizeAsVec2;
			m_framebuffer->resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_camController.onResize(m_viewportSize.x, m_viewportSize.y);
		}
		uint32_t textureID = m_framebuffer->getColourAttachmentRendererID();
		ImGui::Image(textureID, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::onEvent(event::Event& e)
	{
		DD_PROFILE_FUNCTION();
		m_camController.onEvent(e);
	}

}