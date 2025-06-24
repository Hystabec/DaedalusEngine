#include "ddpch.h"
#include "openGLVertexArray.h"

#include <GL/glew.h>
#include "graphics/buffers/utils/shaderDataTypeConversion.h"
#include <vector>

namespace daedalus { namespace graphics { namespace buffers {

	OpenGLVertexArray::OpenGLVertexArray()
	{
		DD_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_renderID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		DD_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_renderID);
	}

	void OpenGLVertexArray::bind() const
	{
		DD_PROFILE_FUNCTION();

		glBindVertexArray(m_renderID);
	}

	void OpenGLVertexArray::unbind() const
	{
		DD_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const shr_ptr<VertexBuffer>& vertexBuffer)
	{
		DD_PROFILE_FUNCTION();

		glBindVertexArray(m_renderID);
		vertexBuffer->bind();

		DD_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex buffer has layout size of 0");

		uint32_t index = 0;
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout.getElements())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.getComponentCount(),
				shader_data_type_to_Open_GL_base_type(element.getBaseDataType()),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.getStide(),
				((const void*)((uint64_t)element.Offset)));	//cast to a uint64_t first to remove compiler warning when casting uint32_t to a void*
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const shr_ptr<IndexBuffer>& indexBuffer)
	{
		DD_PROFILE_FUNCTION();

		glBindVertexArray(m_renderID);
		indexBuffer->bind();
		m_IndexBuffer = indexBuffer;
	}

} } }