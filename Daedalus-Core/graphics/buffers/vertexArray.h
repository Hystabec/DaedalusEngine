#pragma once

#include <vector>
#include <GL/glew.h>

#include "buffer.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	class VertexArray
	{
	private:
		GLuint m_arrayID;
		std::vector<Buffer*> m_buffers;
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(Buffer* buffer, GLuint index);
		void bind() const;
		void unbind() const;
	};

} } }