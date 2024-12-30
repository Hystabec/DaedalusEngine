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

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_renderID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_renderID);
		vertexBuffer->Bind();

		DD_CORE_ASSERT(vertexBuffer->GetLayout().getElements().size(), "Vertex buffer has layout size of 0");

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout.getElements())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.getComponentCount(), Shader_Data_Type_To_Open_GL_Base_Type(element.getBaseDataType()), element.Normalized ? GL_TRUE : GL_FALSE, layout.getStide(), (const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_renderID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

} } }