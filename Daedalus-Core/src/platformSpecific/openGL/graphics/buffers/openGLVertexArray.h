#pragma once

#include "graphics/buffers/vertexArray.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	class OpenGlVertexArray : public VertexArray
	{
	private:
		uint32_t m_renderID;
		uint32_t m_count;
	public:
		OpenGlVertexArray(float* verticies, uint32_t size);
		virtual ~OpenGlVertexArray();

		void Bind() const override;
		void Unbind() const override;
		uint32_t Count() const override { return m_count; };
	};

} } }