#include "ddpch.h"
#include "sprite.h"

namespace daedalusCore { namespace graphics {

		Sprite::Sprite(float x, float y, float width, float height, unsigned int colour)
			: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), colour)
		{
		}

		Sprite::Sprite(float x, float y, float width, float height, Texture* texture)
			: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), 0xffffffff)
		{
			m_texture = texture;
		}

} }