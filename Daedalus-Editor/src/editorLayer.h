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
		//graphics::OrthographicCameraController m_camController;

		Shr_ptr<graphics::Framebuffer> m_framebuffer;

		bool m_viewportFocused = false, m_viewportHovered = false;
		maths::Vec2 m_viewportSize;

		Shr_ptr<scene::Scene> m_activeScene;
		//scene::Entity m_cameraEntity;
		//scene::Entity m_secondCameraEntity;

		SceneHierarchyPanel m_sceneHierarchyPanel;
	};

}