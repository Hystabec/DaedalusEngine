#pragma once

#include <Daedalus.h>

class Layer2D : public daedalusCore::application::Layer
{
public:
	Layer2D();

	void attach() override;
	void detach() override;
	void update(const daedalusCore::application::DeltaTime& dt) override;
	void imGuiRender() override;
	void onEvent(daedalusCore::event::Event& e) override;

private:
	daedalusCore::graphics::OrthographicCameraController m_camController;

	daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> m_texture, m_texture2;

	//alpha testing
	daedalusCore::graphics::primatives2D::QuadProperties ro1, ro2, ro3;

	daedalusCore::graphics::primatives2D::RotatedQuadProperties m_squareProps = { {0.0f, 0.0f, 0.0f}, {0.5f}, 0, {0.8f, 0.2f, 0.3f, 1.0f} };
};