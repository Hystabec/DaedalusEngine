#pragma once

#include <Daedalus.h>

namespace jumper {

	class Platform
	{
	public:
		Platform(daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> platformTexture);

		void render() const;

		void setPosition(const daedalusCore::maths::vec2& pos) { m_renderProps.position = { pos.x, pos.y, -0.1f }; }
		inline const daedalusCore::maths::vec3& getPosition() const { return m_renderProps.position; }
		inline const daedalusCore::maths::vec2& getScale() const { return m_renderProps.size; }

		inline bool getActive() const { return m_active; }
		inline void setActive(bool active) { m_active = active; }

	private:
		daedalusCore::graphics::primatives2D::QuadProperties m_renderProps;
		bool m_active = false;
	};

}