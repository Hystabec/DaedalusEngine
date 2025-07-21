#include "ddpch.h"
#include "openGLUniformBuffer.h"

#include <GL/glew.h>

namespace daedalus::graphics::buffers {

	OpenGlUniformBuffer::OpenGlUniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &m_rendererID);
		glNamedBufferData(m_rendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_rendererID);
	}

	OpenGlUniformBuffer::~OpenGlUniformBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
		m_rendererID = 0;
	}

	void OpenGlUniformBuffer::setData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_rendererID, offset, size, data);
	}

}