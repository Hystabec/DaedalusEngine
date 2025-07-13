#pragma once
#include "panels/sceneHierarchyPanel.h"

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

		void newScene();
		void openScene();
		void saveScene();
		void saveSceneAs();
	private:
		graphics::EditorCamera m_editorCamera;
		Shr_ptr<graphics::Framebuffer> m_framebuffer;

		bool m_viewportFocused = false, m_viewportHovered = false;
		maths::Vec2 m_viewportSize;

		Shr_ptr<scene::Scene> m_activeScene;
		std::string m_currentSceneFilepath = std::string();

		SceneHierarchyPanel m_sceneHierarchyPanel;
		int m_gizmoType = -1;

		scene::Entity m_square;
	};

}