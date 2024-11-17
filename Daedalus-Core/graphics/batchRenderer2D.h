#pragma once

#include "renderer2D.h"
#include "buffers/indexBuffer.h"

namespace daedalusCore { namespace graphics {

#define RENDERER_MAX_SPRITES	60000
#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6

#define SHADER_VERTEX_INDEX		0
#define SHADER_COLOUR_INDEX		1

	class BatchRenderer2D : public Renderer2D
	{
	private:
		GLuint m_VAO;
		GLuint m_BO;
		buffers::IndexBuffer* m_IBO;
		GLsizei m_indexCount;
		VertexData* m_Buffer;
	public:
		BatchRenderer2D();
		~BatchRenderer2D();

		void begin();
		void end();
		void submit(const Renderable2D* renderable) override;
		void render() override;
	};

} }