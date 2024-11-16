#pragma once

#include "../maths/maths.h"

#include "buffers/buffer.h"
#include "buffers/indexBuffer.h"
#include "buffers/vertexArray.h"

#include "shader.h"

namespace daedalusCore { namespace graphics {

	class Renderable2D
	{
	protected:
		maths::vec3 m_position;
		maths::vec2 m_size;
		maths::vec4 m_colour;

		buffers::VertexArray* m_vertexArray;
		buffers::IndexBuffer* m_indexBuffer;
		Shader& m_shader;

	public:
		Renderable2D(maths::vec3 position, maths::vec2 size, maths::vec4 colour, Shader& shader) : m_position(position), m_size(size), m_colour(colour), m_shader(shader)
		{
			m_vertexArray = new buffers::VertexArray();
			GLfloat verticies[] =
			{
				0, 0, 0,
				0, size.y, 0,
				size.x, size.y, 0,
				size.x, 0, 0
			};

			GLfloat colours[] =
			{
				colour.x, colour.y, colour.z, colour.w,
				colour.x, colour.y, colour.z, colour.w,
				colour.x, colour.y, colour.z, colour.w,
				colour.x, colour.y, colour.z, colour.w
			};

			m_vertexArray->addBuffer(new buffers::Buffer(verticies, 4 * 3, 3), 0);
			m_vertexArray->addBuffer(new buffers::Buffer(colours, 4 * 4, 4), 1);

			GLushort indices[] = { 0,1,2,2,3,0 };
			m_indexBuffer = new buffers::IndexBuffer(indices, 6);
		}

		virtual ~Renderable2D()
		{
			delete m_vertexArray;
			delete m_indexBuffer;
		}

		inline const buffers::VertexArray* getVAO() const { return m_vertexArray; }
		inline const buffers::IndexBuffer* getIBO() const { return m_indexBuffer; }
		inline Shader& getShader() const { return m_shader; }

		inline const maths::vec3& getPosition() const { return m_position; }
		inline const maths::vec2& getSize() const { return m_size; }
		inline const maths::vec4& getColour() const { return m_colour; }
	};

} }