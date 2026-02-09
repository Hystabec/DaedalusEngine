#pragma once
#include "panels/sceneHierarchyPanel.h"
#include "panels/contentBrowserPanel.h"

namespace daedalus::editor
{

	class EditorLayer : public application::Layer
	{
	public:
		EditorLayer();

		void attach() override;
		void detach() override;
		void update(const application::DeltaTime& dt) override;
		void imGuiRender() override;
		void onEvent(event::Event& e) override;
		
	private:
		bool onKeyPressed(event::KeyPressedEvent& e);
		bool onMouseButtonPressed(event::MouseButtonPressedEvent& e);
		bool onWindowDrop(event::WindowDropEvent& e);

		void renderOverlays();

		bool canMousePick() const;

		bool newProject();
		bool openProject();
		bool openProject(const std::filesystem::path& path);
		void saveProject();

		void newScene();
		bool openScene();
		bool openScene(AssetHandle handle);
		void saveScene();
		void saveSceneAs();
		void serializeScene(IntrusivePtr<scene::Scene> scene, const std::filesystem::path& path);

		void onScenePlay();
		void onSceneSimulate();
		void onSceneStop();
		void onScenePause();

		void duplicateEntity();

		// UI panels
		void UIToolbar();
	private:
		graphics::EditorCamera m_editorCamera;
		IntrusivePtr<graphics::Framebuffer> m_framebuffer;

		bool m_viewportFocused = false, m_viewportHovered = false;
		maths::Vec2 m_viewportSize;
		maths::Vec2 m_viewportBounds[2];

		IntrusivePtr<scene::Scene> m_activeScene;
		IntrusivePtr<scene::Scene> m_editorScene;
		std::filesystem::path m_currentSceneFilepath = std::filesystem::path();

		SceneHierarchyPanel m_sceneHierarchyPanel;
		int m_gizmoType = -1;
		ContentBrowserPanel m_contentBrowserPanel;

		IntrusivePtr<graphics::Texture2D> m_playIcon, m_stopIcon, m_simulateIcon, m_pauseIcon, m_stepIcon;
		int m_framesPerStep = 1;

		bool m_showColliderOverlay = false;

		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};
		SceneState m_sceneState = SceneState::Edit;

	};

}