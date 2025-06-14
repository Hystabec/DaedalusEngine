#pragma once

#include <Daedalus.h>

namespace jumper {

	class Platform
	{
	public:
		Platform(daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> platformTexture);

		void render() const;

		void setPosition(const daedalusCore::maths::vec2& pos) { m_renderProps.position = { pos.x, pos.y, -0.1f }; }

	private:
		daedalusCore::graphics::primatives2D::QuadProperties m_renderProps;
	};

}