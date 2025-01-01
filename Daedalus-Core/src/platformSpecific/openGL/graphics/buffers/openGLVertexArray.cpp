#include "ddpch.h"
#include "openGLVertexArray.h"

#include <GL/glew.h>
#include "graphics/buffers/utils/shaderDataTypeConversion.h"
#include <vector>

namespace daedalusCore { namespace graphics { namespace buffers {

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_renderID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_renderID);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(m_renderID);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
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
				((void*)element.Offset));
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_renderID);
		indexBuffer->bind();
		m_IndexBuffer = indexBuffer;
	}

} } }