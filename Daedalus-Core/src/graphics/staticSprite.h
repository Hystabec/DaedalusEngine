#pragma once

#include "renderable2D.h"

namespace daedalusCore { namespace graphics {

	class StaticSprite : public Renderable2D
	{
	private:
		buffers::VertexArray* m_vertexArray;
		buffers::IndexBuffer* m_indexBuffer;
		Shader& m_shader;

	public:
		StaticSprite(float x, float y, float width, float height, const maths::vec4& colour, Shader& shader);
		~StaticSprite();

		inline const buffers::VertexArray* getVAO() const { return m_vertexArray; }
		inline const buffers::IndexBuffer* getIBO() const { return m_indexBuffer; }
		inline Shader& getShader() const { return m_shader; }
	};

} }