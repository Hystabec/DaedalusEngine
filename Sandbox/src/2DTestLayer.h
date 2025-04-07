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

	daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> m_texture;

	daedalusCore::maths::vec4 m_colour = { 0.8f, 0.2f,0.3f,1.0f };
	daedalusCore::maths::vec3 m_position = { 0.0f };
	daedalusCore::maths::vec2 m_scale = { 0.5f, 0.5f };
	float m_zRot = 0;
};