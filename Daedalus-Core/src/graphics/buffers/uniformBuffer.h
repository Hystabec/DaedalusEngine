#pragma once

namespace daedalus::graphics::buffers {

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}
		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static Shr_ptr<UniformBuffer> create(uint32_t size, uint32_t binding);
	};

}