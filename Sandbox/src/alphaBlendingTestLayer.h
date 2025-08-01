#pragma once

#include "Daedalus.h"

class AlphaBlendingTestLayer : public daedalus::application::Layer
{
public:
	AlphaBlendingTestLayer();

	void attach() override;
	void detach() override;
	void update(const daedalus::application::DeltaTime& dt) override;
	void imGuiRender() override;
	void onEvent(daedalus::event::Event& e) override;

private:
	daedalus::graphics::OrthographicCameraController m_camController;

	daedalus::Shr_ptr<daedalus::graphics::Texture2D> m_texture, m_texture2;
	daedalus::graphics::primatives2D::QuadProperties ro1, ro2, ro3;
};