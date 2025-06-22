#include "ddpch.h"
#include "subTexture2D.h"

namespace daedalusCore::graphics {

	SubTexture2D::SubTexture2D(const shr_ptr<Texture2D>& texture, const maths::vec2& min, const maths::vec2& max)
		: m_texture(texture)
	{
		m_texCoords[0] = { min.x, min.y };
		m_texCoords[1] = { max.x, min.y };
		m_texCoords[2] = { max.x, max.y };
		m_texCoords[3] = { min.x, max.y };
	}

	shr_ptr<SubTexture2D> SubTexture2D::createFromCoords(const shr_ptr<Texture2D>& texture, const maths::vec2& coords, const maths::vec2& spriteSize)
	{
		maths::vec2 min = { coords.x / texture->getWdith(), coords.y / texture->getHeight() };
		maths::vec2 max = { (coords.x + spriteSize.x) / texture->getWdith(), (coords.y + spriteSize.y) / texture->getHeight() };
		return  create_shr_ptr<SubTexture2D>(texture, min, max);
	}

}