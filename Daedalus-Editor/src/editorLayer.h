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

		bool canMousePick() const;

		void newScene();
		void openScene();
		void openScene(const std::filesystem::path& path);
		void saveScene();
		void saveSceneAs();

		void onScenePlay();
		void onSceneStop();

		// UI panels
		void UIToolbar();
	private:
		graphics::EditorCamera m_editorCamera;
		Shr_ptr<graphics::Framebuffer> m_framebuffer;

		bool m_viewportFocused = false, m_viewportHovered = false;
		maths::Vec2 m_viewportSize;
		maths::Vec2 m_viewportBounds[2];

		Shr_ptr<scene::Scene> m_activeScene;
		std::string m_currentSceneFilepath = std::string();

		SceneHierarchyPanel m_sceneHierarchyPanel;
		int m_gizmoType = -1;
		ContentBrowserPanel m_contentBrowserPanel;

		Shr_ptr<graphics::Texture2D> m_playIcon, m_stopIcon;

		enum class SceneState
		{
			Edit = 0, Play = 1, Pause = 2
		};
		SceneState m_sceneState = SceneState::Edit;
	};

}