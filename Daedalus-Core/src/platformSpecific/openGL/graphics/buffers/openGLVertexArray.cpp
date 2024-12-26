#include "ddpch.h"
#include "openGLVertexArray.h"

#include "GL/glew.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	OpenGlVertexArray::OpenGlVertexArray(float* verticies, uint32_t size)
		: m_count(size)
	{
		glCreateBuffers(1, &m_renderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
	}

	OpenGlVertexArray::~OpenGlVertexArray()
	{
		glDeleteBuffers(1, & m_renderID);
	}

	void OpenGlVertexArray::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
	}

	void OpenGlVertexArray::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} } }