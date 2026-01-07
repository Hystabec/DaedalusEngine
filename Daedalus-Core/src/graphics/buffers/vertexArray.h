#pragma once

#include "vertexBuffer.h"
#include "indexBuffer.h"

namespace daedalus { namespace graphics { namespace buffers {

	class VertexArray : public IntrusiveCounter
	{
	public:
		virtual ~VertexArray() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(const IntrusivePtr<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const IntrusivePtr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<IntrusivePtr<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const IntrusivePtr<IndexBuffer>& getIndexBuffer() const = 0;

		static IntrusivePtr<VertexArray> Create();
	};

} } }