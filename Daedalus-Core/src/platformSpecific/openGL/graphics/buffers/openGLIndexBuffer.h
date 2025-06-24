#pragma once

#include "graphics/buffers/indexBuffer.h"

namespace daedalus { namespace graphics { namespace buffers {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		void bind() const override;
		void unbind() const override;
		uint32_t count() const override { return m_count; };

	private:
		uint32_t m_renderID;
		uint32_t m_count;
	};

} } }