#include "editorpch.h"
#include "EditorLayer.h"
#include <imgui.h>

#include "scene/sceneSerializer.h"
#include "utils/platformUtils.h"

namespace daedalus::editor
{

	EditorLayer::EditorLayer()
	{
	}

	void EditorLayer::attach()
	{
		DD_PROFILE_FUNCTION();

		graphics::FramebufferSpecification fbSpec;
		fbSpec.width = 1600;
		fbSpec.height = 900;
		m_framebuffer = graphics::Framebuffer::create(fbSpec);

		m_activeScene = create_shr_ptr<scene::Scene>();

		m_sceneHierarchyPanel.setContext(m_activeScene);

		/*
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

		m_activeScene->createEntity("Square").addComponent<scene::SpriteRendererComponent>();

		auto scriptCamera = m_activeScene->createEntity("Script Camera");
		scriptCamera.addComponent<scene::CameraComponent>();
		scriptCamera.addComponent<scene::NativeScriptComponent>().bind<CameraController>();
		*/
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
			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					newScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					openScene();
				}

				// TO DO: add save
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					if (!m_currentSceneFilepath.empty())
						saveScene();
					else
						saveSceneAs();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					saveSceneAs();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
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
		//Application::get().getImGuiLayer()->setAllowEvents(m_viewportFocused && m_viewportHovered);

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

		// Shortcut events
		event::EventDispatcher dispatcher(e);
		dispatcher.dispatch<event::KeyPressedEvent>(DD_BIND_EVENT_FUN(EditorLayer::onKeyPressed));

		// Blocking functionality from ImGuiLayer
		if (!(m_viewportFocused && m_viewportHovered))
		{
			ImGuiIO& io = ImGui::GetIO();
			e.setHandled((e.isInCategory(event::EventCategory::Mouse) && io.WantCaptureMouse) || (e.isInCategory(event::EventCategory::Keyboard) && io.WantCaptureMouse));
		}
	}

	bool EditorLayer::onKeyPressed(event::KeyPressedEvent& e)
	{
		using namespace application;

		bool ctrl = Input::getKeyDown(InputCode::Key_Left_Control) || Input::getKeyDown(InputCode::Key_Right_Control);
		bool shift = Input::getKeyDown(InputCode::Key_Left_Shift) || Input::getKeyDown(InputCode::Key_Right_Shift);
		switch ((InputCode)e.getKeyCode())
		{
		case InputCode::Key_N:
		{
			if (ctrl)
				newScene();

			break;
		}
		case InputCode::Key_O:
		{
			if (ctrl)
				openScene();

			break;
		}
		case InputCode::Key_S:
		{
			if (ctrl && shift)
				saveSceneAs();
			else if (ctrl)
			{
				if (!m_currentSceneFilepath.empty())
					saveScene();
				else
					saveSceneAs();
			}
				

			break;
		}
		}

		return false;
	}

	void EditorLayer::newScene()
	{
		m_activeScene = create_shr_ptr<scene::Scene>();
		m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_sceneHierarchyPanel.setContext(m_activeScene);
		m_currentSceneFilepath = std::string();
	}

	void EditorLayer::openScene()
	{
		std::string filepath = utils::FileDialog::openFile("Daedalus Scene (*.daedalus)\0*.daedalus\0");
		if (!filepath.empty())
		{
			m_activeScene = create_shr_ptr<scene::Scene>();

			scene::SceneSerializer serializer(m_activeScene);
			serializer.deserialize(filepath);

			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_sceneHierarchyPanel.setContext(m_activeScene);
			m_currentSceneFilepath = filepath;
		}
	}

	void EditorLayer::saveScene()
	{
		scene::SceneSerializer serializer(m_activeScene);
		serializer.serialize(m_currentSceneFilepath);
		DD_LOG_INFO("Scene saved [{}]", m_currentSceneFilepath);
	}

	void EditorLayer::saveSceneAs()
	{
		std::string filepath = utils::FileDialog::saveFile("Daedalus Scene (*.daedalus)\0*.daedalus\0");
		if (!filepath.empty())
		{
			scene::SceneSerializer serializer(m_activeScene);
			serializer.serialize(filepath);
			m_currentSceneFilepath = filepath;
		}
	}

}