#pragma once

#include "../maths/maths.h"

#include "buffers/buffer.h"
#include "buffers/indexBuffer.h"
#include "buffers/vertexArray.h"

#include "shader.h"

namespace daedalusCore { namespace graphics {

	struct VertexData
	{
		maths::vec3 vertex;
		//maths::vec4 colour;
		unsigned int colour;
	};

	class Renderable2D
	{
	protected:
		maths::vec3 m_position;
		maths::vec2 m_size;
		maths::vec4 m_colour;

		

	public:
		Renderable2D(maths::vec3 position, maths::vec2 size, maths::vec4 colour) : m_position(position), m_size(size), m_colour(colour)
		{
			
		}

		virtual ~Renderable2D() { }

		inline const maths::vec3& getPosition() const { return m_position; }
		inline const maths::vec2& getSize() const { return m_size; }
		inline const maths::vec4& getColour() const { return m_colour; }
	};

} }