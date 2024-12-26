#include "ddpch.h"
#include "openGLIndexBuffer.h"

#include "GL/glew.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t size)
		: m_count(size)
	{
		glCreateBuffers(1, &m_renderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_renderID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} } }