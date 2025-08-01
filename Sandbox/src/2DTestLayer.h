#pragma once

#include "Daedalus.h"

class Layer2D : public daedalus::application::Layer
{
public:
	Layer2D();

	void attach() override;
	void detach() override;
	void update(const daedalus::application::DeltaTime& dt) override;
	void imGuiRender() override;
	void onEvent(daedalus::event::Event& e) override;

private:
	daedalus::graphics::OrthographicCameraController m_camController;

	daedalus::Shr_ptr<daedalus::graphics::Texture2D> m_texture, m_texture2, m_fullSubTextureTesting;
	daedalus::Shr_ptr<daedalus::graphics::SubTexture2D> m_subTexture, m_subTexture2;

	daedalus::graphics::primatives2D::RotatedQuadProperties m_squareProps = { {0.0f, 0.0f, 0.0f}, {0.5f}, 0, {0.8f, 0.2f, 0.3f, 1.0f} };
};