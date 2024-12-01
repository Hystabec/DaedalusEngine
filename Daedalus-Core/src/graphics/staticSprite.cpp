#include "staticSprite.h"

namespace daedalusCore { namespace graphics {

	StaticSprite::StaticSprite(float x, float y, float width, float height, const maths::vec4& colour, Shader& shader) 
		: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), 0xffff00ff), m_shader(shader)
	{
		m_vertexArray = new buffers::VertexArray();
		GLfloat verticies[] =
		{
			0, 0, 0,
			0, height, 0,
			width, height, 0,
			width, 0, 0
		};

		GLfloat colours[] =
		{
			colour.x, colour.y, colour.z, colour.w,
			colour.x, colour.y, colour.z, colour.w,
			colour.x, colour.y, colour.z, colour.w,
			colour.x, colour.y, colour.z, colour.w
		};

		m_vertexArray->addBuffer(new buffers::Buffer(verticies, 4 * 3, 3), 0);
		m_vertexArray->addBuffer(new buffers::Buffer(colours, 4 * 4, 4), 1);

		GLushort indices[] = { 0,1,2,2,3,0 };
		m_indexBuffer = new buffers::IndexBuffer(indices, 6);
	}

	StaticSprite::~StaticSprite()
	{
		delete m_vertexArray;
		delete m_indexBuffer;
	}

} }