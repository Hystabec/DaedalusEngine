#pragma once

#include "graphics/buffers/uniformBuffer.h"

namespace daedalus::graphics::buffers {

	class OpenGlUniformBuffer : public UniformBuffer
	{
	public:
		OpenGlUniformBuffer(uint32_t size, uint32_t binding);
		~OpenGlUniformBuffer() override;

		void setData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t m_rendererID = 0;
	};

}