#pragma once

#include "graphics/buffers/vertexArray.h"

namespace daedalus { namespace graphics { namespace buffers {
	
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void bind() const override;
		void unbind() const override;

		const std::vector<Shr_ptr<VertexBuffer>>& getVertexBuffers() const override { return m_VertexBuffers; }
		const Shr_ptr<IndexBuffer>& getIndexBuffer() const { return m_IndexBuffer; }

		void addVertexBuffer(const Shr_ptr<VertexBuffer>& vertexBuffer) override;
		void setIndexBuffer(const Shr_ptr<IndexBuffer>& indexBuffer) override;

	private:
		std::vector<Shr_ptr<VertexBuffer>> m_VertexBuffers;
		Shr_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_renderID;
	};

} } }