#include "ddpch.h"
#include "openGLRendererAPI.h"

#include <GL/glew.h>

namespace daedalusCore { namespace graphics {

	void OpenGLRendererAPI::init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::setClearColour(const maths::vec4& colour)
	{
		glClearColor(colour.x, colour.y, colour.z, colour.w);
	}

	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::drawIndexedArray(const shr_ptr<graphics::buffers::VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
	}

} }