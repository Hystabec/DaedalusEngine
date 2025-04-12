#pragma once

#include <Daedalus.h>

class AlphaBlendingTestLayer : public daedalusCore::application::Layer
{
public:
	AlphaBlendingTestLayer();

	void attach() override;
	void detach() override;
	void update(const daedalusCore::application::DeltaTime& dt) override;
	void imGuiRender() override;
	void onEvent(daedalusCore::event::Event& e) override;

private:
	daedalusCore::graphics::OrthographicCameraController m_camController;

	daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> m_texture, m_texture2;
	daedalusCore::graphics::primatives2D::QuadProperties ro1, ro2, ro3;
};