#pragma once

#include <GL/glew.h>

namespace daedalusCore {namespace graphics { namespace buffers {

	class IndexBuffer
	{
	private:
		GLuint m_bufferID;
		GLuint m_count;

	public:
		IndexBuffer(GLushort* data, GLsizei count);

		void bind() const;
		void unbind() const;

		inline GLuint getCount() const { return m_count; }
	};

} } }