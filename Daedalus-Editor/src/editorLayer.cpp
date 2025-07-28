#include "editorpch.h"
#include "EditorLayer.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include "scene/sceneSerializer.h"
#include "utils/platformUtils.h"
#include <limits>

namespace daedalus::editor
{

	EditorLayer::EditorLayer()
	{
	}

	void EditorLayer::attach()
	{
		DD_PROFILE_FUNCTION();

		m_editorCamera = graphics::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		graphics::FramebufferSpecification fbSpec;
		fbSpec.attachments = { graphics::FramebufferTextureFormat::RGBA8, graphics::FramebufferTextureFormat::RED_UNSIGNED_INTEGER, graphics::FramebufferTextureFormat::Depth };
		fbSpec.width = 1600;
		fbSpec.height = 900;
		m_framebuffer = graphics::Framebuffer::create(fbSpec);

		m_activeScene = create_shr_ptr<scene::Scene>();

		m_sceneHierarchyPanel.setContext(m_activeScene);

		
		/*class CameraController : public scene::ScriptableEntity
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
		};*/
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
			m_editorCamera.setViewportSize(m_viewportSize.x, m_viewportSize.y);
			m_framebuffer->resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		if(m_viewportFocused)
			m_editorCamera.update(dt);

		graphics::Renderer2D::resetStats();

		m_framebuffer->bind();

		maths::Vec4 colourVec = maths::Vec4(25.0f/255.0f, 25.0f / 255.0f, 25.0f / 255.0f, 1.0f);
		graphics::RenderCommands::setClearColour(colourVec);
		graphics::RenderCommands::clear();

		// entt::null is the same as the max uint value - so if the value
		// from the frame buffer is max uint then it isnt a valid entity
		m_framebuffer->clearAttachment(1, (uint32_t)entt::null);

		// update scene
		m_activeScene->updateEditor(dt, m_editorCamera);

		m_framebuffer->unbind();
	}

	// this function is used to check if the difference between two vec3's is meaningful
	// used to remove any small floating point percition loss.
	// This is mainly used so that the scale doesnt slow decrease overtime when using the gizmos
	static bool is_meaningful_difference(const maths::Vec3& oVal, const maths::Vec3& nVal)
	{
		bool result = false;

		if (abs(oVal.x - nVal.x) > std::numeric_limits<float>::epsilon())
			result = true;

		if (abs(oVal.y - nVal.y) > std::numeric_limits<float>::epsilon())
			result = true;

		if (abs(oVal.z - nVal.z) > std::numeric_limits<float>::epsilon())
			result = true;

		return result;
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
					saveScene();
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
		m_contentBrowserPanel.onImGuiRender();

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

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::get().getImGuiLayer()->setAllowEvents(m_viewportFocused || m_viewportHovered);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		daedalus::maths::Vec2 viewportSizeAsVec2 = { viewportSize.x, viewportSize.y };
		if (m_viewportSize != viewportSizeAsVec2)
		{
			m_viewportSize = viewportSizeAsVec2;
		}
		uint32_t textureID = m_framebuffer->getColourAttachmentRendererID();
		ImGui::Image(textureID, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// TO DO: If the drop target is a valid file, should probably prompt the user
		// to save the current scene
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				std::filesystem::path path = (const wchar_t*)payload->Data;
				if(path.extension().string() == ".Daedalus")
				openScene(path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		scene::Entity selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();

		// runtime camera from entity
		//auto cameraEntity = m_activeScene->getPrimaryCameraEntity();
		//const auto& cc = cameraEntity.getComponent<scene::CameraComponent>();
		//const maths::Mat4& cameraProjection = cc.Camera.getProjection();
		//maths::Mat4 cameraView = maths::Mat4::invert(cameraEntity.getComponent<scene::TransformComponent>().getTransform())

		if (selectedEntity && m_gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y, m_viewportBounds[1].x - m_viewportBounds[0].x, m_viewportBounds[1].y - m_viewportBounds[0].y);

			const maths::Mat4& cameraProjection = m_editorCamera.getProjection();
			maths::Mat4 cameraView = m_editorCamera.getViewMatrix();

			auto& tc = selectedEntity.getComponent<scene::TransformComponent>();
			maths::Mat4 transform = tc.getTransform();

			bool snap = application::Input::getKeyDown(application::InputCode::Key_Left_Shift) || application::Input::getKeyDown(application::InputCode::Key_Right_Shift);
			float snapValue = 0.5f; // Snap to 0.5 for translate/scale
			if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)	// snap to 45 degrees for rotation
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(cameraView, cameraProjection,
				(ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, transform,
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				maths::Vec3 position, rotation, scale;
				if (maths::Mat4::decomposeTransform(transform, position, rotation, scale))
				{
					if(is_meaningful_difference(tc.position, position))
						tc.position = position;

					if (is_meaningful_difference(tc.rotation, rotation))
					{
						maths::Vec3 deltaRotation = rotation - tc.rotation;
						tc.rotation += deltaRotation;
					}

					if(is_meaningful_difference(tc.scale, scale))
						tc.scale = scale;
				}
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::onEvent(event::Event& e)
	{
		DD_PROFILE_FUNCTION();

		m_editorCamera.onEvent(e);

		// Shortcut events
		event::EventDispatcher dispatcher(e);
		dispatcher.dispatch<event::KeyPressedEvent>(DD_BIND_EVENT_FUN(EditorLayer::onKeyPressed));
		dispatcher.dispatch<event::MouseButtonPressedEvent>(DD_BIND_EVENT_FUN(EditorLayer::onMouseButtonPressed));

		//// Blocking functionality from ImGuiLayer
		//if (!(m_viewportFocused && m_viewportHovered))
		//{
		//	ImGuiIO& io = ImGui::GetIO();
		//	e.setHandled((e.isInCategory(event::EventCategory::Mouse) && io.WantCaptureMouse) || (e.isInCategory(event::EventCategory::Keyboard) && io.WantCaptureMouse));
		//}
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
				saveScene();

			break;
		}

			// Gizmos
		case InputCode::Key_Q:
		{
			if(shift)
				m_gizmoType = -1;
			break;
		}
		case InputCode::Key_W:
		{
			if(shift)
				m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case InputCode::Key_E:
		{
			if(shift)
				m_gizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case InputCode::Key_R:
		{
			if(shift)
				m_gizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		}

		return false;
	}

	bool EditorLayer::onMouseButtonPressed(event::MouseButtonPressedEvent& e)
	{
		if (e.getButtonCode() == application::InputCode::Mouse_Button_Left)
		{
			if (canMousePick())
			{
				auto [mx, my] = ImGui::GetMousePos();
				mx -= m_viewportBounds[0].x;
				my -= m_viewportBounds[0].y;
				maths::Vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
				my = viewportSize.y - my;

				int mouseX = (int)mx;
				int mouseY = (int)my;

				// binding and unbinding is kind of expensive but i dont think i have much choice
				m_framebuffer->bind();
				uint32_t pixelData = m_framebuffer->readPixel(1, mouseX, mouseY);
				m_framebuffer->unbind();

				// if the returned value is the max uint value then it will equal entt::null 
				scene::Entity pickedEntity = pixelData != entt::null ? scene::Entity((entt::entity)pixelData, m_activeScene.get()) : scene::Entity();

				m_sceneHierarchyPanel.setSelectedEntity(pickedEntity);
			}
		}

		return false;
	}

	bool EditorLayer::canMousePick() const
	{
		// Check if the camera is being used
		// TO DO: Expand this to actually check if the camera is being manipulated
		// no just if left control is pressed 
		bool cameraInUse = application::Input::getKeyDown(application::InputCode::Key_Left_Control);

		return m_viewportHovered && m_viewportFocused && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver() && !cameraInUse;
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
			openScene(filepath);
		}
	}

	void EditorLayer::openScene(const std::filesystem::path& path)
	{
		auto newScene = create_shr_ptr<scene::Scene>();

		scene::SceneSerializer serializer(newScene);
		if (serializer.deserialize(path.string()))
		{
			m_activeScene = newScene;
			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_sceneHierarchyPanel.setContext(m_activeScene);
			m_currentSceneFilepath = path.string();
		}
	}

	void EditorLayer::saveScene()
	{
		// If the current scene wasnt opened/saved from/to a file, open the
		// saveAs dialog
		if (m_currentSceneFilepath.empty())
		{
			saveSceneAs();
			return;
		}

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