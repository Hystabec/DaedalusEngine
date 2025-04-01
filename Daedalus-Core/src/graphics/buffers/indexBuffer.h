#pragma once

namespace daedalusCore {namespace graphics { namespace buffers {

	class IndexBuffer
	{
	public:
		~IndexBuffer() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual uint32_t count() const = 0;

		static shr_ptr<IndexBuffer> create(uint32_t* indices, uint32_t size);
	};

} } }