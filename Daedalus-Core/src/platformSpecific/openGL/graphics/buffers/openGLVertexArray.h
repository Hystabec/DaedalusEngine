#pragma once

#include "graphics/buffers/vertexArray.h"

namespace daedalus { namespace graphics { namespace buffers {
	
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void bind() const override;
		void unbind() const override;

		const std::vector<IntrusivePtr<VertexBuffer>>& getVertexBuffers() const override { return m_VertexBuffers; }
		const IntrusivePtr<IndexBuffer>& getIndexBuffer() const { return m_IndexBuffer; }

		void addVertexBuffer(const IntrusivePtr<VertexBuffer>& vertexBuffer) override;
		void setIndexBuffer(const IntrusivePtr<IndexBuffer>& indexBuffer) override;

	private:
		std::vector<IntrusivePtr<VertexBuffer>> m_VertexBuffers;
		IntrusivePtr<IndexBuffer> m_IndexBuffer;
		uint32_t m_renderID;
	};

} } }