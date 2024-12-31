#pragma once
#include "bufferLayout.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual uint32_t count() const = 0;

		virtual const BufferLayout& getLayout() const = 0;
		virtual void setLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* create(float* verticies, uint32_t size);
	};

} } }