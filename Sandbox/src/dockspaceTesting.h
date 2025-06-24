#pragma once
#include "Daedalus.h"

class EditorLayer : public daedalusCore::application::Layer
{
public:
	EditorLayer();

	void attach() override;
	void detach() override;
	void update(const daedalusCore::application::DeltaTime& dt) override;
	void imGuiRender() override;
	void onEvent(daedalusCore::event::Event& e) override;

private:
	daedalusCore::graphics::OrthographicCameraController m_camController;
	daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> m_texture;

	daedalusCore::shr_ptr<daedalusCore::graphics::Framebuffer> m_framebuffer;
};
