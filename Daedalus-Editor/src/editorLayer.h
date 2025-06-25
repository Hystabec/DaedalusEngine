#pragma once
#include "Daedalus.h"

namespace daedalus
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
		graphics::OrthographicCameraController m_camController;

		shr_ptr<graphics::Framebuffer> m_framebuffer;

		bool m_viewportFocused = false, m_viewportHovered = false;
		maths::vec2 m_viewportSize;
	};

}