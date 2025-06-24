#include "ddpch.h"
#include "openGLIndexBuffer.h"

#include "GL/glew.h"

namespace daedalus { namespace graphics { namespace buffers {

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_count(count)
	{
		DD_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_renderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		DD_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_renderID);
	}

	void OpenGLIndexBuffer::bind() const
	{
		DD_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID);
	}

	void OpenGLIndexBuffer::unbind() const
	{
		DD_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} } }