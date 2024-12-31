#pragma once

#include "graphics/buffers/vertexArray.h"

namespace daedalusCore { namespace graphics { namespace buffers {
	
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void bind() const override;
		void unbind() const override;

		const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override { return m_VertexBuffers; }
		const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return m_IndexBuffer; }

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_renderID;
	};

} } }