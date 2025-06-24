#pragma once

#include "vertexBuffer.h"
#include "indexBuffer.h"

namespace daedalus { namespace graphics { namespace buffers {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(const shr_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const shr_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<shr_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const shr_ptr<IndexBuffer>& getIndexBuffer() const = 0;

		static shr_ptr<VertexArray> Create();
	};

} } }