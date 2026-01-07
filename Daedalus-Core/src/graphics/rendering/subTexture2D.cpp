#include "ddpch.h"
#include "subTexture2D.h"

namespace daedalus::graphics {

	SubTexture2D::SubTexture2D(const IntrusivePtr<Texture2D>& texture, const maths::Vec2& min, const maths::Vec2& max)
		: m_texture(texture)
	{
		m_texCoords[0] = { min.x, min.y };
		m_texCoords[1] = { max.x, min.y };
		m_texCoords[2] = { max.x, max.y };
		m_texCoords[3] = { min.x, max.y };
	}

	IntrusivePtr<SubTexture2D> SubTexture2D::createFromCoords(const IntrusivePtr<Texture2D>& texture, const maths::Vec2& coords, const maths::Vec2& spriteSize)
	{
		maths::Vec2 min = { coords.x / texture->getWidth(), coords.y / texture->getHeight() };
		maths::Vec2 max = { (coords.x + spriteSize.x) / texture->getWidth(), (coords.y + spriteSize.y) / texture->getHeight() };
		return  make_intrusive_ptr<SubTexture2D>(texture, min, max);
	}

}