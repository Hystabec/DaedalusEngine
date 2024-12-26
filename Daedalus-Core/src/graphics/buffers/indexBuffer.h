#pragma once

namespace daedalusCore {namespace graphics { namespace buffers {

	class IndexBuffer
	{
	public:
		~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t Count() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	};

} } }