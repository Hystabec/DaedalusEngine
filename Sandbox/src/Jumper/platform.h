#pragma once

#include <Daedalus.h>

namespace jumper {

	class Platform
	{
	public:
		Platform(daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> platformTexture);

		void render() const;

	private:
		daedalusCore::graphics::primatives2D::QuadProperties m_renderProps;
	};

}