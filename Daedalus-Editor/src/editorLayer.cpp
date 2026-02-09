#include "editorpch.h"
#include "EditorLayer.h"

#include "scene/sceneSerializer.h"
#include "utils/platformUtils.h"
#include "scripting/scriptEngine.h"
#include "graphics/rendering/font.h"

#include "asset/importers/sceneImporter.h"
#include "asset/importers/textureImporter.h"
#include "asset/assetManager.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include <limits>

namespace daedalus::editor
{
	static constexpr ImVec4 standardTint(1.0f, 1.0f, 1.0f, 1.0f);
	static constexpr ImVec4 fadeTint(0.5f, 0.5f, 0.5f, 1.0f);

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

		m_playIcon = TextureImporter::loadTexture2D("resources\\icons\\playButtonIcon.png");
		m_stopIcon = TextureImporter::loadTexture2D("resources\\icons\\stopButtonIcon.png");
		m_simulateIcon = TextureImporter::loadTexture2D("resources\\icons\\simulateButtonIcon.png");
		m_pauseIcon = TextureImporter::loadTexture2D("resources\\icons\\pauseButtonIcon.png");
		m_stepIcon = TextureImporter::loadTexture2D("resources\\icons\\stepButtonIcon.png");

		m_editorScene = make_intrusive_ptr<scene::Scene>();
		m_activeScene = m_editorScene;

		auto& commandLineArgs = Application::get().getSpecification().commandLineArgs;
		if (commandLineArgs.count > 1)
		{
			// sandboxProject\\sandbox.ddproj
			auto projectFilePath = commandLineArgs[1];
			openProject(projectFilePath);
		}
		else
		{
			// NOTE: This will prompt the user to open a project if they dont
			// then they will be prompted to select a location to make a new peoject
			if (!openProject())
				if (!newProject())
					Application::get().close();
		}

		m_sceneHierarchyPanel.setContext(m_activeScene);

		if(m_currentSceneFilepath == std::filesystem::path())
			Application::get().getWindow()->setWindowName("Daedalus Editor");
		else
			Application::get().getWindow()->setWindowName("Daedalus Editor - " + m_currentSceneFilepath.stem().string());
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
				if (ImGui::MenuItem("Open Project ...", "Ctrl+O"))
					openProject();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					newScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					saveScene();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					saveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
					Application::get().close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::BeginMenu("Selection Gizmo"))
				{
					if (ImGui::MenuItem("None##seclctionGizmo", "Shift+Q", m_gizmoType == -1))
						m_gizmoType = -1;

					if (ImGui::MenuItem("Translate##seclctionGizmo", "Shift+W", m_gizmoType == ImGuizmo::OPERATION::TRANSLATE))
						m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;

					const static int RotateGizmoValue = ImGuizmo::OPERATION::ROTATE_X | ImGuizmo::OPERATION::ROTATE_Y | ImGuizmo::OPERATION::ROTATE_Z;
					if (ImGui::MenuItem("Rotate##seclctionGizmo", "Shift+E", m_gizmoType == RotateGizmoValue))
						m_gizmoType = RotateGizmoValue;

					if (ImGui::MenuItem("Scale##seclctionGizmo", "Shift+R", m_gizmoType == ImGuizmo::OPERATION::SCALE))
						m_gizmoType = ImGuizmo::OPERATION::SCALE;

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Toggle collider overlay", "Alt+C", m_showColliderOverlay))
				{
					m_showColliderOverlay = !m_showColliderOverlay;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug"))
			{
				bool sceneIsPaused = m_activeScene->isPaused();
				float imageSize = ImGui::GetIO().FontDefault->FontSize;

				{
					bool isUsable = m_sceneState == SceneState::Edit || (sceneIsPaused && m_sceneState == SceneState::Play);
					ImGui::Image((ImTextureID)m_playIcon->getRendererID(), ImVec2(imageSize, imageSize), ImVec2(0, 0), ImVec2(1, 1),
						isUsable ? standardTint : fadeTint);
					ImGui::SameLine();
					if (ImGui::MenuItem("Play", nullptr, false, isUsable))
					{
						if (m_sceneState == SceneState::Edit)
							onScenePlay();
						else if (sceneIsPaused)
						{
							m_activeScene->setPaused(false);
						}
					}
				}

				{
					bool isUsable = m_sceneState == SceneState::Edit || (sceneIsPaused && m_sceneState == SceneState::Simulate);
					ImGui::Image((ImTextureID)m_simulateIcon->getRendererID(), ImVec2(imageSize, imageSize), ImVec2(0, 0), ImVec2(1, 1),
						isUsable ? standardTint : fadeTint);
					ImGui::SameLine();
					if (ImGui::MenuItem("Simulate", nullptr, false, isUsable))
					{
						if (m_sceneState == SceneState::Edit)
							onSceneSimulate();
						else if (sceneIsPaused)
						{
							m_activeScene->setPaused(false);
						}
					}
				}

				{
					bool isUsable = m_sceneState != SceneState::Edit && !sceneIsPaused;
					ImGui::Image((ImTextureID)m_pauseIcon->getRendererID(), ImVec2(imageSize, imageSize), ImVec2(0, 0), ImVec2(1, 1),
						isUsable ? standardTint : fadeTint);
					ImGui::SameLine();
					if (ImGui::MenuItem("Pause", nullptr, false, isUsable))
					{
						if (isUsable)
							m_activeScene->setPaused(true);
					}
				}

				{
					bool isUsable = m_sceneState != SceneState::Edit;
					ImGui::Image((ImTextureID)m_stopIcon->getRendererID(), ImVec2(imageSize, imageSize), ImVec2(0, 0), ImVec2(1, 1),
						isUsable ? standardTint : fadeTint);
					ImGui::SameLine();
					if (ImGui::MenuItem("Stop", nullptr, false, isUsable))
					{
						if (m_sceneState == SceneState::Play || m_sceneState == SceneState::Simulate)
							onSceneStop();
					}
				}

				{
					bool isUsable = sceneIsPaused;
					ImGui::Image((ImTextureID)m_stepIcon->getRendererID(), ImVec2(imageSize, imageSize), ImVec2(0, 0), ImVec2(1, 1),
						isUsable ? standardTint : fadeTint);
					ImGui::SameLine();
					if (ImGui::MenuItem("Step", nullptr, false, isUsable))
					{
						m_activeScene->step(m_framesPerStep);
					}
				}

				{
					if (ImGui::BeginMenu("Options"))
					{
						// NOTE: A width of 50.0f is a bit of a magic number,
						// it just resulted in a nice size. Might consider changing later
						ImGui::PushItemWidth(50.0f);
						ImGui::DragInt("Frames per step", &m_framesPerStep);
						ImGui::PopItemWidth();

						ImGui::EndMenu();
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload assembly"))
				{
					scripting::ScriptEngine::reloadAssembly();
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

		ImGui::Separator();

		ImGui::Text("Text draw calls: %d", stats.textDrawCalls);
		ImGui::Text("Text Quads: %d", stats.textQuadCount);
		ImGui::Text("Text Vertices: %d", stats.getTotalTextVertexCount());
		ImGui::Text("Text Indices: %d", stats.getTotalTextIndexCount());
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
					std::filesystem::path extension = utils::FileSystem::fileExtensionToLower(path.extension());
					if (extension == ".ddscene")
					{
						auto relativePath = std::filesystem::relative(path, Project::getActiveAssetDirectory());
						AssetHandle handle = Project::getActive()->getEditorAssetManager()->importAsset(relativePath);
						if (handle)
							openScene(handle);
					}
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
		// NOTE: This will need to be updated if/when more controls are added
		constexpr int numberOfControls = 5;

		bool sceneIsPaused = m_activeScene->isPaused();
		{
			bool isUsable = m_sceneState == SceneState::Edit || (sceneIsPaused && m_sceneState == SceneState::Play);
			IntrusivePtr<graphics::Texture2D> icon = m_playIcon;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - ((size * numberOfControls) * 0.5f));
			if (ImGui::ImageButton("##playButton", (ImTextureID)icon->getRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0),
				!isUsable ? fadeTint : standardTint))
			{
				if (m_sceneState == SceneState::Edit)
					onScenePlay();
				else if (sceneIsPaused)
				{
					m_activeScene->setPaused(false);
				}
			}
			if (isUsable)
			{
				if (ImGui::BeginItemTooltip())
				{
					ImGui::Text(sceneIsPaused ? "Resume" : "Start");
					ImGui::EndTooltip();
				}
			}
		}
		ImGui::SameLine();
		{
			bool isUsable = m_sceneState == SceneState::Edit || (sceneIsPaused && m_sceneState == SceneState::Simulate);
			IntrusivePtr<graphics::Texture2D> icon = m_simulateIcon;
			if (ImGui::ImageButton("##simulatePlayButton", (ImTextureID)icon->getRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0),
				!isUsable ? fadeTint : standardTint))
			{
				if (m_sceneState == SceneState::Edit)
					onSceneSimulate();
				else if (sceneIsPaused)
				{
					m_activeScene->setPaused(false);
				}
			}
			if (isUsable)
			{
				if (ImGui::BeginItemTooltip())
				{
					ImGui::Text(sceneIsPaused ? "Resume simulation" : "Start simulation");
					ImGui::EndTooltip();
				}
			}
		}
		ImGui::SameLine();
		{
			bool isUsable = m_sceneState != SceneState::Edit && !sceneIsPaused;
			IntrusivePtr<graphics::Texture2D> icon = m_pauseIcon;
			if (ImGui::ImageButton("##pauseButton", (ImTextureID)icon->getRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0),
				!isUsable ? fadeTint : standardTint))
			{
				if (isUsable)
					m_activeScene->setPaused(true);
			}
			if(isUsable)
			{
				if (ImGui::BeginItemTooltip())
				{
					ImGui::Text("Pause");
					ImGui::EndTooltip();
				}
			}
		}
		ImGui::SameLine();
		{
			bool isUsable = m_sceneState != SceneState::Edit;
			IntrusivePtr<graphics::Texture2D> icon = m_stopIcon;
			if (ImGui::ImageButton("##stopButton", (ImTextureID)icon->getRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0),
				!isUsable ? fadeTint : standardTint))
			{
				if (m_sceneState == SceneState::Play || m_sceneState == SceneState::Simulate)
					onSceneStop();
			}
			if (isUsable)
			{
				if (ImGui::BeginItemTooltip())
				{
					ImGui::Text("Stop");
					ImGui::EndTooltip();
				}
			}
		}
		ImGui::SameLine();
		{
			bool isUsable = sceneIsPaused;
			IntrusivePtr<graphics::Texture2D> icon = m_stepIcon;
			if (ImGui::ImageButton("##stepButton", (ImTextureID)icon->getRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0),
				!isUsable ? fadeTint : standardTint))
			{
				m_activeScene->step(m_framesPerStep);
			}
			if (isUsable)
			{
				if (ImGui::BeginItemTooltip())
				{
					ImGui::Text("Next step");
					ImGui::EndTooltip();
				}
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
		dispatcher.dispatch<event::WindowDropEvent>(DD_BIND_EVENT_FUN(EditorLayer::onWindowDrop));
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
				openProject();

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

	bool EditorLayer::onWindowDrop(event::WindowDropEvent& e)
	{
		// TO DO: if a project is dropped in, open it / promt the user to save and open
		std::vector<std::filesystem::path> droppedPaths = e.getPaths();
		for (std::filesystem::path path : droppedPaths)
		{
			if (path.extension() == ".ddproj")
			{
				// TO DO: prompt user to save the current project
				openProject(path);
			}
		}
		//AssetManager::importAsset();

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

	bool EditorLayer::newProject()
	{
		// NOTE: New project could check in the selected directory if there is a 
		// .ddproj file and if so load that project
		if (m_sceneState != SceneState::Edit)
			return false;

		std::filesystem::path directoryPath = utils::FileDialog::selectFolder(L"Select new project directory...");
		if (!directoryPath.empty())
		{
			Project::newProject(directoryPath, "NewProject");

			scripting::ScriptEngine::init();
			newScene();
			m_contentBrowserPanel.setProjectAssetDirectory(Project::getActiveAssetDirectory());

			return true;
		}

		return false;
	}

	bool EditorLayer::openProject()
	{
		if (m_sceneState != SceneState::Edit)
			return false;

		// open file returns a string - here is getting cast/constucted into a filepath
		std::filesystem::path filepath = utils::FileDialog::openFile({ L"Daedalus Project", L"*.ddproj" });
		if (!filepath.empty())
		{
			return openProject(filepath);
		}
		
		return false;
	}

	bool EditorLayer::openProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			scripting::ScriptEngine::init();

			AssetHandle startScene = Project::getActive()->getConfig().startScene;

			if (!startScene)
				newScene();
			else
			{
				if (!openScene(startScene))
					newScene();
			}

			m_contentBrowserPanel.setProjectAssetDirectory(Project::getActiveAssetDirectory());
			return true;
		}

		return false;
	}

	void EditorLayer::saveProject()
	{
		// Project::saveActive();
	}

	void EditorLayer::newScene()
	{
		if (m_sceneState != SceneState::Edit)
			return;

		m_editorScene = make_intrusive_ptr<scene::Scene>();

		m_activeScene = m_editorScene;
		m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_sceneHierarchyPanel.setContext(m_activeScene);
		m_currentSceneFilepath = std::filesystem::path();
		Application::get().getWindow()->setWindowName("Daedalus Editor");
	}

	bool EditorLayer::openScene()
	{
		if (m_sceneState != SceneState::Edit)
			return false;

		// open file returns a string - here is getting cast/constucted into a filepath
		std::filesystem::path filepath = utils::FileDialog::openFile({ L"Daedalus Scene", L"*.ddscene" });
		if (!filepath.empty())
		{
			AssetHandle sceneHandle = Project::getActive()->getEditorAssetManager()->importAsset(filepath);
			return openScene(sceneHandle);
		}

		return false;
	}

	bool EditorLayer::openScene(AssetHandle handle)
	{
		DD_CORE_ASSERT(handle);

		if (m_sceneState != SceneState::Edit)
			return false;

		 auto ROScene = AssetManager::getAsset<scene::Scene>(handle);

		if (ROScene)
		{
			auto newScene = scene::Scene::copy(ROScene);
			m_editorScene = newScene;

			m_editorScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_sceneHierarchyPanel.setContext(m_editorScene);
			m_currentSceneFilepath = Project::getActive()->getEditorAssetManager()->getFilepath(handle);
			Application::get().getWindow()->setWindowName("Daedalus Editor - " + m_currentSceneFilepath.stem().string());

			m_activeScene = m_editorScene;

			return true;
		}

		return false;
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

		const wchar_t* defaultFileName;
		if (m_currentSceneFilepath.empty())
			defaultFileName = L"newScene.ddscene";
		else
			defaultFileName = m_currentSceneFilepath.filename().c_str();

		std::filesystem::path filepath = utils::FileDialog::saveFile({ L"Daedalus Scene", L"*.ddscene" }, defaultFileName);
		if (!filepath.empty())
		{
			// NOTE: Could just change the file extension regardless of what it is
			// currently it only sets to .ddscene if it is empty
			if (filepath.extension().empty())
				filepath.replace_extension(".ddscene");

			serializeScene(m_activeScene, filepath);
			m_currentSceneFilepath = filepath;
			Application::get().getWindow()->setWindowName("Daedalus Editor - " + m_currentSceneFilepath.stem().string());
		}
	}

	void EditorLayer::serializeScene(IntrusivePtr<scene::Scene> scene, const std::filesystem::path& path)
	{
		auto relPath = std::filesystem::relative(path, Project::getActiveAssetDirectory());
		if (relPath.empty())
			relPath = path;
		SceneImporter::saveScene(scene, Project::getActiveAssetDirectory() / relPath);
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

	void EditorLayer::onScenePause()
	{
		if (m_sceneState == SceneState::Edit)
			return;

		m_activeScene->setPaused(true);
	}

	void EditorLayer::duplicateEntity()
	{
		if (m_sceneState != SceneState::Edit)
			return;

		scene::Entity selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();
		if (selectedEntity)
		{
			scene::Entity newEntity = m_editorScene->duplicateEntity(selectedEntity);
			m_sceneHierarchyPanel.setSelectedEntity(newEntity);
		}
	}

}