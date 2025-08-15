#include "editorpch.h"
#include "EditorLayer.h"

#include "scene/sceneSerializer.h"
#include "utils/platformUtils.h"

#include <imgui.h>
#include <ImGuizmo.h>
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

		m_playIcon = graphics::Texture2D::create("resources\\icons\\playButtonIcon.png");
		m_stopIcon = graphics::Texture2D::create("resources\\icons\\stopButtonIcon.png");
		m_simulateIcon = graphics::Texture2D::create("resources\\icons\\simulateButtonIcon.png");

		m_editorScene = create_shr_ptr<scene::Scene>();
		m_activeScene = m_editorScene;

		auto& commandLineArgs = Application::get().getSpecification().commandLineArgs;
		if (commandLineArgs.count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			openScene(sceneFilePath);
		}

		m_sceneHierarchyPanel.setContext(m_activeScene);

		if(m_currentSceneFilepath == std::filesystem::path())
			Application::get().getWindow()->setWindowName("Daedalus Editor");
		else
			Application::get().getWindow()->setWindowName("Daedalus Editor - " + m_currentSceneFilepath.stem().string());

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

#ifndef DD_DISTRO
		graphics::Renderer2D::resetStats();
#endif

		m_framebuffer->bind();

		maths::Vec4 colourVec = maths::Vec4(25.0f / 255.0f, 25.0f / 255.0f, 25.0f / 255.0f, 1.0f);
		graphics::RenderCommands::setClearColour(colourVec);
		graphics::RenderCommands::clear();

		// entt::null is the same as the max uint value - so if the value
		// from the frame buffer is max uint then it isnt a valid entity
		m_framebuffer->clearAttachment(1, (uint32_t)entt::null);

		// update scene
		switch (m_sceneState)
		{
		case SceneState::Edit:
		{
			if (m_viewportFocused)
				m_editorCamera.update(dt);

			m_activeScene->updateEditor(dt, m_editorCamera);
			break;
		}
		case SceneState::Simulate:
		{
			if (m_viewportFocused)
				m_editorCamera.update(dt);

			m_activeScene->updateSimulation(dt, m_editorCamera);
			break;
		}
		case SceneState::Play:
		{
			m_activeScene->updateRuntime(dt);
			break;
		}
		}

		renderOverlays();

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

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Toggle collider overlay", "Alt+C", m_showColliderOverlay))
				{
					m_showColliderOverlay = !m_showColliderOverlay;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		m_sceneHierarchyPanel.onImGuiRender();
		m_contentBrowserPanel.onImGuiRender();

		ImGui::Begin("Stats");
#ifndef DD_DISTRO
		auto stats = graphics::Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());
#endif
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
		//Application::get().getImGuiLayer()->setAllowEvents(m_viewportFocused || m_viewportHovered);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		daedalus::maths::Vec2 viewportSizeAsVec2 = { viewportSize.x, viewportSize.y };
		if (m_viewportSize != viewportSizeAsVec2)
		{
			m_viewportSize = viewportSizeAsVec2;
		}
		uint32_t textureID = m_framebuffer->getColourAttachmentRendererID();
		ImGui::Image(textureID, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// only allow drag and drop(scene) and gizmos if current state is edit
		if (m_sceneState == SceneState::Edit)
		{
			// TO DO: If the drop target is a valid file, should probably prompt the user
			// to save the current scene
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					std::filesystem::path path = (const wchar_t*)payload->Data;
					if (path.extension().string() == ".Daedalus")
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
						if (is_meaningful_difference(tc.position, position))
							tc.position = position;

						if (is_meaningful_difference(tc.rotation, rotation))
						{
							maths::Vec3 deltaRotation = rotation - tc.rotation;
							tc.rotation += deltaRotation;
						}

						if (is_meaningful_difference(tc.scale, scale))
							tc.scale = scale;
					}
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();

		UIToolbar();
	}

	void EditorLayer::UIToolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		const auto& colours = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colours[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, buttonHovered.w / 0.5f));
		const auto& buttonActive = colours[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, buttonActive.w / 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		{
			Shr_ptr<graphics::Texture2D> icon = m_sceneState == SceneState::Play ? m_stopIcon : m_playIcon;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
			if (ImGui::ImageButton("##playStopButton", (ImTextureID)icon->getRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0),
				m_sceneState == SceneState::Simulate ? ImVec4(0.5f, 0.5f, 0.5f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f)))
			{
				if (m_sceneState == SceneState::Edit)
					onScenePlay();
				else if (m_sceneState == SceneState::Play)
					onSceneStop();
			}
			if (ImGui::BeginItemTooltip())
			{
				ImGui::Text(m_sceneState == SceneState::Play ? "Stop" : "Start");
				ImGui::EndTooltip();
			}
		}
		ImGui::SameLine();
		{
			Shr_ptr<graphics::Texture2D> icon = m_sceneState == SceneState::Simulate ? m_stopIcon : m_simulateIcon;
			//ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
			if (ImGui::ImageButton("##simulatePlayStopButton", (ImTextureID)icon->getRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0),
				m_sceneState == SceneState::Play ? ImVec4(0.5f, 0.5f, 0.5f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f)))
			{
				if (m_sceneState == SceneState::Edit)
					onSceneSimulate();
				else if (m_sceneState == SceneState::Simulate)
					onSceneStop();
			}
			if (ImGui::BeginItemTooltip())
			{
				ImGui::Text(m_sceneState == SceneState::Simulate ? "Stop simulation" : "Start simulation");
				ImGui::EndTooltip();
			}
		}
		ImGui::End();

		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(3);
	}

	void EditorLayer::onEvent(event::Event& e)
	{
		DD_PROFILE_FUNCTION();

		// only run if in the viewport
		if(m_viewportFocused || m_viewportHovered)
			m_editorCamera.onEvent(e);

		// Shortcut events
		event::EventDispatcher dispatcher(e);
		dispatcher.dispatch<event::KeyPressedEvent>(DD_BIND_EVENT_FUN(EditorLayer::onKeyPressed));
		dispatcher.dispatch<event::MouseButtonPressedEvent>(DD_BIND_EVENT_FUN(EditorLayer::onMouseButtonPressed));
	}

	bool EditorLayer::onKeyPressed(event::KeyPressedEvent& e)
	{
		using namespace application;

		// TO DO: The ctrl + KEY should be usable when focusing and hovering any window
		// but gizmos camera etc. should still be blocked

		// This should probably be organized alphabetically, as more shotcut get added
		// its going to get more confusing with the current order.

		bool ctrl = Input::getKeyDown(InputCode::Key_Left_Control) || Input::getKeyDown(InputCode::Key_Right_Control);
		bool shift = Input::getKeyDown(InputCode::Key_Left_Shift) || Input::getKeyDown(InputCode::Key_Right_Shift);
		bool alt = Input::getKeyDown(InputCode::Key_Left_Alt) || Input::getKeyDown(InputCode::Key_Right_Alt);
		
		// general editor shortcuts
		switch ((InputCode)e.getKeyCode())
		{
		case InputCode::Key_C:
		{
			if (alt)
				m_showColliderOverlay = !m_showColliderOverlay;
			break;
		}
		case InputCode::Key_D:
		{
			if (ctrl)
				duplicateEntity();

			break;
		}
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
		}

		if(m_viewportFocused || m_viewportHovered)
		{
			// shortcuts that need window to be focused (e.g. editor camera)
			switch ((InputCode)e.getKeyCode())
			{
			case InputCode::Key_E:
			{
				if (shift)
					m_gizmoType = ImGuizmo::OPERATION::ROTATE_X | ImGuizmo::OPERATION::ROTATE_Y | ImGuizmo::OPERATION::ROTATE_Z;
					//m_gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case InputCode::Key_Q:
			{
				if (shift)
					m_gizmoType = -1;
				break;
			}
			case InputCode::Key_R:
			{
				if (shift)
					m_gizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
			case InputCode::Key_W:
			{
				if (shift)
					m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			}
		}

		return false;
	}

	bool EditorLayer::onMouseButtonPressed(event::MouseButtonPressedEvent& e)
	{
		if (e.getButtonCode() == application::InputCode::Mouse_Button_Left && m_viewportHovered)
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

	void EditorLayer::renderOverlays()
	{
		using namespace graphics;


		switch (m_sceneState)
		{
		case EditorLayer::SceneState::Edit:
		case EditorLayer::SceneState::Simulate:
		{
			Renderer2D::begin(m_editorCamera);
			break;
		}
		case EditorLayer::SceneState::Play:
		{
			scene::Entity camera = m_activeScene->getPrimaryCameraEntity();
			DD_ASSERT(camera, "No primary camera");
			Renderer2D::begin(camera.getComponent<scene::CameraComponent>().camera, camera.getComponent<scene::TransformComponent>().getTransform());
			break;
		}
		}

		if (m_showColliderOverlay)
		{
			m_activeScene->getAllEntitiesWith<scene::TransformComponent, scene::CircleCollider2DComponent>().each([](const scene::TransformComponent& tc, const scene::CircleCollider2DComponent& cc2d)
				{
					maths::Vec3 rotatedOffset = cc2d.offset;
					rotatedOffset = maths::experimental::rotate_vec3_by_quaternion(maths::experimental::Quaternion(maths::Vec3(0.0f, 0.0f, tc.rotation.z)), rotatedOffset);

					maths::Mat4 transform = maths::Mat4::translate(tc.position + maths::Vec3(rotatedOffset.x, rotatedOffset.y, 0.001f))
						* maths::Mat4::scale(/*Assumes scale is uniform*/ tc.scale.x * (cc2d.radius * 2.0f));

					Renderer2D::drawCircle(transform, maths::Vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.05f);
				});

			m_activeScene->getAllEntitiesWith<scene::TransformComponent, scene::BoxCollider2DComponent>().each([](const scene::TransformComponent& tc, const scene::BoxCollider2DComponent& bc2d)
				{
					maths::Vec3 rotatedOffset = bc2d.offset;
					rotatedOffset = maths::experimental::rotate_vec3_by_quaternion(maths::experimental::Quaternion(maths::Vec3(0.0f, 0.0f, tc.rotation.z)), rotatedOffset);

					maths::Mat4 transform = maths::Mat4::translate(tc.position + maths::Vec3(rotatedOffset.x, rotatedOffset.y, 0.001f))
						* maths::Mat4::rotate(tc.rotation.z, maths::Vec3(0.0f, 0.0f, 1.0f))
						* maths::Mat4::scale(tc.scale * maths::Vec3(bc2d.size * 2.0f, 1.0f));

					Renderer2D::drawRect(transform, maths::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
				});
		}

		// selected entity outline
		if (scene::Entity selectedEntity = m_sceneHierarchyPanel.getSelectedEntity())
		{
			// This is something of a temporary solution,
			// as it can only draw boxs with 2D objects.
			// A better (but currently massivly overkill solution) could be to
			// use jump flooding, which would also work with 3D.
			// Example of jump flooding: https://www.youtube.com/watch?v=QjrAJwaUy64

			auto& tc = selectedEntity.getTransformComponent();

			graphics::Renderer2D::drawRect(tc.getTransform() * maths::Mat4::translate(maths::Vec3(0.0f, 0.0f, 0.001f)), maths::Vec4(0.76f, 0.00f, 0.10f, 1.0f));
		}

		Renderer2D::end();
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
		if (m_sceneState != SceneState::Edit)
			return;

		m_editorScene = create_shr_ptr<scene::Scene>();

		m_activeScene = m_editorScene;
		m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_sceneHierarchyPanel.setContext(m_activeScene);
		m_currentSceneFilepath = std::filesystem::path();
		Application::get().getWindow()->setWindowName("Daedalus Editor");
	}

	void EditorLayer::openScene()
	{
		if (m_sceneState != SceneState::Edit)
			return;

		// open file returns a string - here is getting cast/constucted into a filepath
		std::filesystem::path filepath = utils::FileDialog::openFile("Daedalus Scene (*.daedalus)\0*.daedalus\0");
		if (!filepath.empty())
		{
			openScene(filepath);
		}
	}

	void EditorLayer::openScene(const std::filesystem::path& path)
	{
		if (m_sceneState != SceneState::Edit)
			return;

		auto newScene = create_shr_ptr<scene::Scene>();

		scene::SceneSerializer serializer(newScene);
		if (serializer.deserialize(path.string()))
		{
			m_editorScene = newScene;

			m_editorScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_sceneHierarchyPanel.setContext(m_editorScene);
			m_currentSceneFilepath = path.string();
			Application::get().getWindow()->setWindowName("Daedalus Editor - " + m_currentSceneFilepath.stem().string());

			m_activeScene = m_editorScene;
		}
	}

	void EditorLayer::saveScene()
	{
		// could just save the edit scnene instead of doing this check,
		// but this seems better
		if (m_sceneState != SceneState::Edit)
			return;

		// If the current scene wasnt opened/saved from/to a file, open the
		// saveAs dialog
		if (m_currentSceneFilepath.empty())
		{
			saveSceneAs();
			return;
		}

		serializeScene(m_activeScene, m_currentSceneFilepath);
	}

	void EditorLayer::saveSceneAs()
	{
		// could just save the edit scnene instead of doing this check,
		// but this seems better
		if (m_sceneState != SceneState::Edit)
			return;

		std::filesystem::path filepath = utils::FileDialog::saveFile("Daedalus Scene (*.daedalus)\0*.daedalus\0");
		if (!filepath.empty())
		{
			serializeScene(m_activeScene, filepath);
			m_currentSceneFilepath = filepath;
			Application::get().getWindow()->setWindowName("Daedalus Editor - " + m_currentSceneFilepath.stem().string());
		}
	}

	void EditorLayer::serializeScene(Shr_ptr<scene::Scene> scene, const std::filesystem::path& path)
	{
		scene::SceneSerializer serializer(scene);
		serializer.serialize(path.string());
		DD_LOG_INFO("Scene saved [{}]", path.string());
	}

	void EditorLayer::onScenePlay()
	{
		m_sceneState = SceneState::Play;

		// Make the active scene a copy of the editor scene
		// now becomes the runtime scene
		m_activeScene = scene::Scene::copy(m_editorScene);
		m_activeScene->onRuntimeStart();

		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::onSceneSimulate()
	{
		m_sceneState = SceneState::Simulate;

		// Make the active scene a copy of the editor scene
		// now becomes the runtime scene
		m_activeScene = scene::Scene::copy(m_editorScene);
		m_activeScene->onSimulateStart();

		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::onSceneStop()
	{
		DD_CORE_ASSERT(m_sceneState == SceneState::Play || m_sceneState == SceneState::Simulate);
		m_sceneState == SceneState::Play ? m_activeScene->onRutimeStop() : m_activeScene->onSimulateStop();

		m_sceneState = SceneState::Edit;
		
		m_activeScene = m_editorScene;

		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::duplicateEntity()
	{
		if (m_sceneState != SceneState::Edit)
			return;

		scene::Entity selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();
		if (selectedEntity)
			m_editorScene->duplicateEntity(selectedEntity);
	}

}