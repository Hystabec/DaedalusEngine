#pragma once

namespace daedalusCore { namespace graphics { namespace buffers {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexArray* Create(float* verticies, uint32_t size);
	};

} } }