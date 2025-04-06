#include "ddpch.h"
#include "renderer2D.h"

#include "../buffers/vertexArray.h"
#include "../shaders/shader.h"
#include "renderCommands.h"

namespace daedalusCore { namespace graphics {

	struct Renderer2DStorage
	{
		bool beginCalled = false;
		shr_ptr<buffers::VertexArray> quadVertexArray;
		shr_ptr<Shader> flatColourShader;
	};

	static Renderer2DStorage* s_data = nullptr;

	void Renderer2D::init()
	{
		DD_CORE_ASSERT(!s_data, "Renderer2D::init called twice");

		s_data = new Renderer2DStorage();

		s_data->quadVertexArray = buffers::VertexArray::Create();
		float sqrVerts[4 * 3] = {
			-1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f
		};

		buffers::BufferLayout sqrLayout =
		{
			{ DD_BUFFERS_VEC3, "a_position" }
		};

		shr_ptr<buffers::VertexBuffer> sqrVertBuff(buffers::VertexBuffer::create(sqrVerts, sizeof(sqrVerts)));
		sqrVertBuff->setLayout(sqrLayout);
		s_data->quadVertexArray->addVertexBuffer(sqrVertBuff);
		uint32_t sqrIndices[3 * 2] = { 0, 1, 2, 2, 3, 0 };
		shr_ptr<buffers::IndexBuffer> sqrIndexBuff(buffers::IndexBuffer::create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t)));
		s_data->quadVertexArray->setIndexBuffer(sqrIndexBuff);

		s_data->flatColourShader = Shader::create("resources/shaders/flatShader.glsl");
	}

	void Renderer2D::shutdown()
	{
		delete s_data;
		s_data = nullptr;
	}

	void Renderer2D::begin(const OrthographicCamera& othoCamera)
	{
		DD_CORE_ASSERT(!(s_data->beginCalled), "Renderer2D::end not called");
		s_data->beginCalled = true;

		s_data->flatColourShader->enable();
		s_data->flatColourShader->setUniformMat4(othoCamera.getProjectViewMatrix(), "u_projView");
		//s_data->flatColourShader->setUniformMat4(maths::mat4(1.0f), "u_transform");
	}

	void Renderer2D::end()
	{
		DD_CORE_ASSERT((s_data->beginCalled), "Renderer2D::begin not called");

		s_data->beginCalled = false;
	}

	void Renderer2D::drawQuad(const maths::vec2& position, const maths::vec2& size, const maths::vec4& colour)
	{
		drawQuad({ position.x, position.y, 0 }, size, 0, colour);
	}

	void Renderer2D::drawQuad(const maths::vec2& position, const maths::vec2& size, const float& rotation, const maths::vec4& colour)
	{
		drawQuad({ position.x, position.y, 0 }, size, rotation, colour);
	}

	void Renderer2D::drawQuad(const maths::vec3& position, const maths::vec2& size, const maths::vec4& colour)
	{
		drawQuad(position, size, 0, colour);
	}

	void Renderer2D::drawQuad(const maths::vec3& position, const maths::vec2& size, const float& rotation, const maths::vec4& colour)
	{
		DD_CORE_ASSERT((s_data->beginCalled), "Renderer2D::begin not called");

		s_data->flatColourShader->enable();
		s_data->flatColourShader->setUniform4f(colour, "u_colour");
		s_data->flatColourShader->setUniformMat4(maths::mat4::translate(position) * maths::mat4::rotate(rotation, { 0,0,1 }) * maths::mat4::scale({ size.x, size.y, 1 }), "u_transform");

		s_data->quadVertexArray->bind();
		RenderCommands::drawIndexed(s_data->quadVertexArray);
	}

} }