#include "ddpch.h"
#include "openGLVertexBuffer.h"

#include "GL/glew.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size)
		: m_count(size)
	{
		DD_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_renderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGlVertexBuffer::OpenGlVertexBuffer(float* verticies, uint32_t size)
		: m_count(size)
	{
		DD_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_renderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
	}

	OpenGlVertexBuffer::~OpenGlVertexBuffer()
	{
		DD_PROFILE_FUNCTION();

		glDeleteBuffers(1, & m_renderID);
	}

	void OpenGlVertexBuffer::bind() const
	{
		DD_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
	}

	void OpenGlVertexBuffer::unbind() const
	{
		DD_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGlVertexBuffer::setData(const void* data, uint32_t size)
	{
		DD_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

} } }