#pragma once
#include "bufferLayout.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t Count() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexArray* Create(float* verticies, uint32_t size);
	};

} } }