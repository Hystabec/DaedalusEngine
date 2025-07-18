#pragma once
#include "bufferLayout.h"

namespace daedalus { namespace graphics { namespace buffers {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual uint32_t count() const = 0;

		virtual const BufferLayout& getLayout() const = 0;
		virtual void setLayout(const BufferLayout& layout) = 0;
		
		virtual void setData(const void* data, uint32_t size) = 0;

		static Shr_ptr<VertexBuffer> create(uint32_t size);
		static Shr_ptr<VertexBuffer> create(float* verticies, uint32_t size);
	};

} } }