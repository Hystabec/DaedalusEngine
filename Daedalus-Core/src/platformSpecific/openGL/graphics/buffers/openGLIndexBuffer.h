#pragma once

#include "graphics/buffers/indexBuffer.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t m_renderID;
		uint32_t m_count;
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;
		uint32_t Count() const override { return m_count; };
	};

} } }