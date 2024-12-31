#include "ddpch.h"
#include "openGLVertexBuffer.h"

#include "GL/glew.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	OpenGlVertexBuffer::OpenGlVertexBuffer(float* verticies, uint32_t size)
		: m_count(size)
	{
		glCreateBuffers(1, &m_renderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
	}

	OpenGlVertexBuffer::~OpenGlVertexBuffer()
	{
		glDeleteBuffers(1, & m_renderID);
	}

	void OpenGlVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
	}

	void OpenGlVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} } }