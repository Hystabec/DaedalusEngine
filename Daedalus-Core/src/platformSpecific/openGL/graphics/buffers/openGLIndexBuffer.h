#pragma once

#include "graphics/buffers/indexBuffer.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t m_renderID;
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;
	};

} } }