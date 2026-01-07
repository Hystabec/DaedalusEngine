#pragma once

#include "maths/vec2.h"
#include "texture.h"

namespace daedalus::graphics {

	class SubTexture2D : public IntrusiveCounter
	{
	public:
		SubTexture2D(const IntrusivePtr<Texture2D>& texture, const maths::Vec2& min, const maths::Vec2& max);

		const IntrusivePtr<Texture2D> getTexture() const { return m_texture; }
		const maths::Vec2* getTextureCoords() const { return m_texCoords; }

		static IntrusivePtr<SubTexture2D> createFromCoords(const IntrusivePtr<Texture2D>& texture, const maths::Vec2& coords, const maths::Vec2& spriteSize);
	private:
		IntrusivePtr<Texture2D> m_texture;
		maths::Vec2 m_texCoords[4];
	};

}