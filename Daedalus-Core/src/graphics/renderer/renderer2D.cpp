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

	void Renderer2D::drawQuad(const primatives2D::QuadProperties& quadProps)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT((s_data->beginCalled), "Renderer2D::begin not called");

		s_data->defaultShader->setUniform4f(quadProps.colour, "u_colour");
		s_data->defaultShader->setUniform1f(quadProps.tilingFactor, "u_tilingFactor");

		s_data->defaultShader->setUniformMat4(
			maths::mat4::translate(quadProps.position) 
			* maths::mat4::scale({ quadProps.size.x, quadProps.size.y, 1 })
			, "u_transform");

		if (quadProps.texture != nullptr)
			quadProps.texture->bind();	//might need to also check if the shr_ptr is still valid
		else
			s_data->whiteTexture->bind();

		s_data->quadVertexArray->bind();
		RenderCommands::drawIndexed(s_data->quadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const primatives2D::RotatedQuadProperties& rotQuadProps)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT((s_data->beginCalled), "Renderer2D::begin not called");

		s_data->defaultShader->setUniform4f(rotQuadProps.colour, "u_colour");
		s_data->defaultShader->setUniform1f(rotQuadProps.tilingFactor, "u_tilingFactor");

		//rotation assumed to be degrees
		s_data->defaultShader->setUniformMat4(
			maths::mat4::translate(rotQuadProps.position)
			* maths::mat4::rotate(rotQuadProps.rotation, { 0,0,1 })
			* maths::mat4::scale({ rotQuadProps.size.x, rotQuadProps.size.y, 1 })
			, "u_transform");

		if (rotQuadProps.texture != nullptr)
			rotQuadProps.texture->bind();	//might need to also check if the shr_ptr is still valid
		else
			s_data->whiteTexture->bind();

		s_data->quadVertexArray->bind();
		RenderCommands::drawIndexed(s_data->quadVertexArray);
	}

} }