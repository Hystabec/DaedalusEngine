#include "platform.h"

namespace jumper
{

	Platform::Platform(daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> platformTexture)
	{
		m_renderProps.texture = platformTexture;
		m_renderProps.position = { 0.0f, 0.0f, -0.1f };
		m_renderProps.size = { 0.5f, 0.25f };
	}

	void Platform::render() const
	{
		daedalusCore::graphics::Renderer2D::drawQuad(m_renderProps);
	}

}