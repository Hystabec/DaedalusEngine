#pragma once
#include "Daedalus.h"

class EditorLayer : public daedalus::application::Layer
{
public:
	EditorLayer();

	void attach() override;
	void detach() override;
	void update(const daedalus::application::DeltaTime& dt) override;
	void imGuiRender() override;
	void onEvent(daedalus::event::Event& e) override;

private:
	daedalus::graphics::OrthographicCameraController m_camController;
	daedalus::shr_ptr<daedalus::graphics::Texture2D> m_texture;

	daedalus::shr_ptr<daedalus::graphics::Framebuffer> m_framebuffer;
};
