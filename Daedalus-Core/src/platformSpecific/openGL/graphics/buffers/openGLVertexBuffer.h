#pragma once

#include "graphics/buffers/vertexBuffer.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	class OpenGlVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t m_renderID;
		uint32_t m_count;
		BufferLayout m_layout;
	public:
		OpenGlVertexBuffer(float* verticies, uint32_t size);
		virtual ~OpenGlVertexBuffer();

		void Bind() const override;
		void Unbind() const override;
		uint32_t Count() const override { return m_count; };

		const BufferLayout& GetLayout() const override { return m_layout; };
		void SetLayout(const BufferLayout& layout) override { m_layout = layout; };
	};

} } }