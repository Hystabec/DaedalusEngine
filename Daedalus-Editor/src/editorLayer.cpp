#include "editorpch.h"
#include "EditorLayer.h"
#include <imgui.h>

namespace daedalus::editor
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

		m_cameraEntity = m_activeScene->createEntity("Camera A");
		m_cameraEntity.addComponent<scene::CameraComponent>();

		m_secondCameraEntity = m_activeScene->createEntity("Camera B");
		m_secondCameraEntity.addComponent<scene::CameraComponent>().Primary = false;

		class CameraController : public scene::ScriptableEntity
		{
		public:
			void onCreate()
			{
			}

			void onDestroy()
			{
			}

			void onUpdate(const application::DeltaTime& dt)
			{
				auto& position = getComponent<scene::TransformComponent>().Position;
				float speed = 5.0f;

				if (application::Input::getKeyDown(application::InputCode::Key_W))
					position.y += speed * dt;
				if (application::Input::getKeyDown(application::InputCode::Key_S))
					position.y -= speed * dt;

				if (application::Input::getKeyDown(application::InputCode::Key_A))
					position.x -= speed * dt;
				if (application::Input::getKeyDown(application::InputCode::Key_D))
					position.x += speed * dt;
			}
		};

		m_secondCameraEntity.addComponent<scene::NativeScriptComponent>().bind<CameraController>();

		auto redSquare = m_activeScene->createEntity("Red Square");
		redSquare.addComponent<scene::SpriteRendererComponent>(maths::Vec4{ 0.8f, 0.2f, 0.2f, 1.0f });

		auto greenSquare = m_activeScene->createEntity("Green Square");
		greenSquare.addComponent<scene::SpriteRendererComponent>(maths::Vec4{ 0.2f, 0.8f, 0.2f, 1.0f });

		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::detach()
	{
		DD_PROFILE_FUNCTION();
	}

	void EditorLayer::update(const application::DeltaTime& dt)
	{
		DD_PROFILE_FUNCTION();

		// Resize
		if (graphics::FramebufferSpecification spec = m_framebuffer->getSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&	// zero sized framebuffer is invalid
			(spec.width != m_viewportSize.x || spec.height != m_viewportSize.y))
		{
			m_framebuffer->resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_camController.onResize(m_viewportSize.x, m_viewportSize.y);
			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		if(m_viewportFocused)
			m_camController.update(dt);

		graphics::Renderer2D::resetStats();

		m_framebuffer->bind();
		graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		graphics::RenderCommands::clear();

		// update scene
		m_activeScene->update(dt);

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

		m_sceneHierarchyPanel.onImGuiRender();

		ImGui::Begin("Stats");

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
		daedalus::maths::Vec2 viewportSizeAsVec2 = { viewportSize.x, viewportSize.y };
		if (m_viewportSize != viewportSizeAsVec2)
		{
			m_viewportSize = viewportSizeAsVec2;
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