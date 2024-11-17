#include "batchRenderer2D.h"

namespace daedalusCore { namespace graphics {

	BatchRenderer2D::BatchRenderer2D() : m_indexCount(0), m_Buffer(nullptr)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_BO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_BO);
		glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glEnableVertexAttribArray(SHADER_COLOUR_INDEX);
		glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(0));
		glVertexAttribPointer(SHADER_COLOUR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::colour)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

		int offset = 0;
		for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[i]	   = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_IBO = new buffers::IndexBuffer(indices, RENDERER_INDICES_SIZE);
		glBindVertexArray(0);
	}

	BatchRenderer2D::~BatchRenderer2D()
	{
		delete m_IBO;
		glDeleteBuffers(1, &m_BO);
	}

	void BatchRenderer2D::begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BO);
		m_Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void BatchRenderer2D::end()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BatchRenderer2D::submit(const Renderable2D* renderable)
	{
		const maths::vec3& position = renderable->getPosition();
		const maths::vec2& size = renderable->getSize();
		const maths::vec4& colour = renderable->getColour();

		int r = colour.x * 255;
		int g = colour.y * 255;
		int b = colour.z * 255;
		int a = colour.w * 255;
		unsigned int c = a << 24 | b << 16 | g << 6 | r;

		m_Buffer->vertex = position;
		m_Buffer->colour = c;
		m_Buffer++;

		m_Buffer->vertex = maths::vec3(position.x, position.y + size.y, position.z);
		m_Buffer->colour = c;
		m_Buffer++;

		m_Buffer->vertex = maths::vec3(position.x + size.x, position.y + size.y, position.z);
		m_Buffer->colour = c;
		m_Buffer++;

		m_Buffer->vertex = maths::vec3(position.x + size.x, position.y, position.z);
		m_Buffer->colour = c;
		m_Buffer++;

		m_indexCount += 6;
	}

	void BatchRenderer2D::render()
	{
		glBindVertexArray(m_VAO);
		m_IBO->bind();

		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, NULL);

		m_IBO->unbind();
		glBindVertexArray(0);

		m_indexCount = 0;
	}

} }