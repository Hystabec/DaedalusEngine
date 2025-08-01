#pragma once

namespace daedalus {namespace graphics { namespace buffers {

	// Only supports 32-bit index buffers
	class IndexBuffer
	{
	public:
		~IndexBuffer() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual uint32_t count() const = 0;

		static Shr_ptr<IndexBuffer> create(uint32_t* indices, uint32_t count);
	};

} } }