#pragma once

#include "maths/vec2.h"
#include "texture.h"

namespace daedalus::graphics {

	class SubTexture2D
	{
	public:
		SubTexture2D(const shr_ptr<Texture2D>& texture, const maths::vec2& min, const maths::vec2& max);

		const shr_ptr<Texture2D> getTexture() const { return m_texture; }
		const maths::vec2* getTextureCoords() const { return m_texCoords; }

		static shr_ptr<SubTexture2D> createFromCoords(const shr_ptr<Texture2D>& texture, const maths::vec2& coords, const maths::vec2& spriteSize);
	private:
		shr_ptr<Texture2D> m_texture;
		maths::vec2 m_texCoords[4];
	};

}