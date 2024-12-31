#pragma once

#include "graphics/buffers/vertexBuffer.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	class OpenGlVertexBuffer : public VertexBuffer
	{
	public:
		OpenGlVertexBuffer(float* verticies, uint32_t size);
		virtual ~OpenGlVertexBuffer();

		void bind() const override;
		void unbind() const override;
		uint32_t count() const override { return m_count; };

		const BufferLayout& getLayout() const override { return m_layout; };
		void setLayout(const BufferLayout& layout) override { m_layout = layout; };

	private:
		uint32_t m_renderID;
		uint32_t m_count;
		BufferLayout m_layout;
	};

} } }