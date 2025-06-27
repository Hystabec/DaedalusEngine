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

		m_cameraEntity = m_activeScene->createEntity("Camera Entity");
		m_cameraEntity.addComponent<scene::CameraComponent>();

		m_secondCameraEntity = m_activeScene->createEntity("Second Camera Entity");
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
				auto& transform = getComponent<scene::TransformComponent>().Transform;
				float speed = 5.0f;

				if (application::Input::getKeyDown(application::InputCode::Key_W))
					transform.columns[3].y += speed * dt;
				if (application::Input::getKeyDown(application::InputCode::Key_S))
					transform.columns[3].y -= speed * dt;

				if (application::Input::getKeyDown(application::InputCode::Key_A))
					transform.columns[3].x -= speed * dt;
				if (application::Input::getKeyDown(application::InputCode::Key_D))
					transform.columns[3].x += speed * dt;
			}
		};

		m_secondCameraEntity.addComponent<scene::NativeScriptComponent>().bind<CameraController>();

		auto square = m_activeScene->createEntity("Square");
		square.addComponent<scene::SpriteRendererComponent>(maths::Vec4{ 0.8f, 0.2f, 0.2f, 1.0f });
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

		//graphics::Renderer2D::begin(m_camController.getCamera());

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

		//graphics::Renderer2D::end();
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

		ImGui::Separator();

		static bool primaryCameraA = true;
		if (ImGui::Checkbox("Camera A", &primaryCameraA))
		{
			m_cameraEntity.getComponent<scene::CameraComponent>().Primary = primaryCameraA;
			m_secondCameraEntity.getComponent<scene::CameraComponent>().Primary = !primaryCameraA;
		}

		{
			auto& camera = m_secondCameraEntity.getComponent<scene::CameraComponent>().Camera;
			float orthoSize = camera.getOrthographicSize();
			if (ImGui::DragFloat("Second camera ortho size", &orthoSize))
				camera.setOrthographicSize(orthoSize);
		}


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