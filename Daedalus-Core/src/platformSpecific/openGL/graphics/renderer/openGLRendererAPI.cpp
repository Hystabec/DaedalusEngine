#include "ddpch.h"
#include "openGLRendererAPI.h"

#include <GL/glew.h>

namespace daedalus { namespace graphics {

	void OpenGLRendererAPI::init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::setClearColour(const maths::Vec4& colour)
	{
		glClearColor(colour.x, colour.y, colour.z, colour.w);
	}

	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::drawIndexed(const Shr_ptr<graphics::buffers::VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->bind();
		uint32_t count = indexCount ? indexCount : vertexArray->getIndexBuffer()->count();

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

} }