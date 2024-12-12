#pragma once

#include "maths/maths.h"
#include "texture.h"
#include "renderer2D.h"

namespace daedalusCore { namespace graphics {

	struct VertexData
	{
		maths::vec3 vertex;
		maths::vec2 uv;
		float tid;
		unsigned int colour;
	};

	class Renderable2D
	{
	protected:
		maths::vec3 m_position;
		maths::vec2 m_size;
		unsigned int m_colour;
		std::vector<maths::vec2> m_UV;
		Texture* m_texture;
	protected:
		Renderable2D() { setUVDefaults(); }

	public:
		Renderable2D(maths::vec3 position, maths::vec2 size, unsigned int colour) : m_position(position), m_size(size), m_colour(colour)
		{
			setUVDefaults();
		}

		virtual ~Renderable2D() { }

		virtual void submit(Renderer2D* renderer) const
		{
			renderer->submit(this);
		}

		void setColour(unsigned int colour) { m_colour = colour; }
		void setColour(const maths::vec4& colour)
		{
			int r = colour.x * 255;
			int g = colour.y * 255;
			int b = colour.z * 255;
			int a = colour.w * 255;
			m_colour = a << 24 | b << 16 | g << 8 | r;
		}

		inline const maths::vec3& getPosition() const { return m_position; }
		inline const maths::vec2& getSize() const { return m_size; }
		inline const unsigned int getColour() const { return m_colour; }
		inline const std::vector<maths::vec2>& getUV() const { return m_UV; }
		inline const GLuint getTextureID() const { return m_texture == nullptr ? 0 : m_texture->getID(); }

	private:
		void setUVDefaults()
		{
			m_UV.push_back(maths::vec2(0, 0));
			m_UV.push_back(maths::vec2(0, 1));
			m_UV.push_back(maths::vec2(1, 1));
			m_UV.push_back(maths::vec2(1, 0));
		}
	};

} }