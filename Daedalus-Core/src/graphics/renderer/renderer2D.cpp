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
		shr_ptr<Shader> defaultShader;
		shr_ptr<Texture2D> whiteTexture;
	};

	static Renderer2DStorage* s_data = nullptr;

	void Renderer2D::init()
	{
		DD_PROFILE_FUNCTION();

		DD_CORE_ASSERT(!s_data, "Renderer2D::init called twice");

		s_data = new Renderer2DStorage();

		s_data->quadVertexArray = buffers::VertexArray::Create();
		float sqrVerts[4 * 5] = {
			-1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 1.0f
		};

		buffers::BufferLayout sqrLayout =
		{
			{ DD_BUFFERS_VEC3, "a_position" },
			{ DD_BUFFERS_VEC2, "a_texCoord" }
		};

		shr_ptr<buffers::VertexBuffer> sqrVertBuff(buffers::VertexBuffer::create(sqrVerts, sizeof(sqrVerts)));
		sqrVertBuff->setLayout(sqrLayout);
		s_data->quadVertexArray->addVertexBuffer(sqrVertBuff);
		uint32_t sqrIndices[3 * 2] = { 0, 1, 2, 2, 3, 0 };
		shr_ptr<buffers::IndexBuffer> sqrIndexBuff(buffers::IndexBuffer::create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t)));
		s_data->quadVertexArray->setIndexBuffer(sqrIndexBuff);

		s_data->whiteTexture = graphics::Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data->whiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

		s_data->defaultShader = Shader::create("../Daedalus-Core/resources/shaders/default2DShader.glsl");	//relative directory could be an issue 
		s_data->defaultShader->enable();
		s_data->defaultShader->setUniform1i(0, "u_texture");
	}

	void Renderer2D::shutdown()
	{
		DD_PROFILE_FUNCTION();

		delete s_data;
		s_data = nullptr;
	}

	void Renderer2D::begin(const OrthographicCamera& othoCamera)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT(!(s_data->beginCalled), "Renderer2D::end not called");
		s_data->beginCalled = true;

		s_data->defaultShader->enable();
		s_data->defaultShader->setUniformMat4(othoCamera.getProjectViewMatrix(), "u_projView");
	}

	void Renderer2D::end()
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT((s_data->beginCalled), "Renderer2D::begin not called");

		s_data->beginCalled = false;
	}

	void Renderer2D::drawQuad(const maths::vec2& position, const maths::vec2& size, const float& rotation, const maths::vec4& colour)
	{
		drawQuad({ position.x, position.y, 0 }, size, rotation, colour);
	}

	void Renderer2D::drawQuad(const maths::vec3& position, const maths::vec2& size, const float& rotation, const maths::vec4& colour)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT((s_data->beginCalled), "Renderer2D::begin not called");

		s_data->defaultShader->setUniform4f(colour, "u_colour");
		s_data->defaultShader->setUniformMat4(maths::mat4::translate(position) * maths::mat4::rotate(rotation, { 0,0,1 }) * maths::mat4::scale({ size.x, size.y, 1 }), "u_transform");

		s_data->whiteTexture->bind();

		s_data->quadVertexArray->bind();
		RenderCommands::drawIndexed(s_data->quadVertexArray);
	}

	void Renderer2D::drawQuad(const maths::vec2& position, const maths::vec2& size, const float& rotation, const shr_ptr<graphics::Texture2D>& texture, const maths::vec4& colour)
	{
		drawQuad({ position.x, position.y, 0 }, size, rotation, texture, colour);
	}

	void Renderer2D::drawQuad(const maths::vec3& position, const maths::vec2& size, const float& rotation, const shr_ptr<graphics::Texture2D>& texture, const maths::vec4& colour)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT((s_data->beginCalled), "Renderer2D::begin not called");

		s_data->defaultShader->setUniform4f(colour, "u_colour");
		s_data->defaultShader->setUniformMat4(maths::mat4::translate(position) * maths::mat4::rotate(rotation, { 0,0,1 }) * maths::mat4::scale({ size.x, size.y, 1 }), "u_transform");

		texture->bind();

		s_data->quadVertexArray->bind();
		RenderCommands::drawIndexed(s_data->quadVertexArray);
	}

} }