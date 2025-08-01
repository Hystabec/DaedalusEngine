#include "ddpch.h"
#include "renderer2D.h"

#include "../buffers/vertexArray.h"
#include "../buffers/uniformBuffer.h"
#include "../shaders/shader.h"
#include "renderCommands.h"

#include "utils/findFileLocation.h"

namespace daedalus { namespace graphics {

	struct QuadVertex
	{
		maths::Vec3 position;
		maths::Vec4 colour;
		maths::Vec2 texCoord;
		float texIndex;
		float tilingFactor;

		// Editor only
		uint32_t entityID;
	};

	struct Renderer2DData
	{
		static const uint32_t maxQuads = 10000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32; // TO DO: RenderCaps

		bool beginCalled = false;
		Shr_ptr<buffers::VertexArray> quadVertexArray;
		Shr_ptr<buffers::VertexBuffer> quadVertexBuffer;
		Shr_ptr<Shader> defaultShader;
		Shr_ptr<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Shr_ptr<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 = white texture

		maths::Vec3 quadVertexPositions[4];

#ifndef DD_DISTRO
		Renderer2D::Statistics stats;
#endif

		struct CameraData
		{
			maths::Mat4 viewProjection;
		};
		CameraData cameraBuffer;
		Shr_ptr<buffers::UniformBuffer> cameraUniformBuffer;
	};

	static Renderer2DData s_data;

	void Renderer2D::init()
	{
		DD_PROFILE_FUNCTION();

		s_data.quadVertexArray = buffers::VertexArray::Create();

		s_data.quadVertexBuffer = buffers::VertexBuffer::create(s_data.maxVertices * sizeof(QuadVertex));
		s_data.quadVertexBuffer->setLayout({
			{ DD_BUFFERS_VEC3,	 "a_position"	  },
			{ DD_BUFFERS_VEC4,	 "a_colour"		  },
			{ DD_BUFFERS_VEC2,	 "a_texCoord"	  },
			{ DD_BUFFERS_FLOAT,	 "a_texIndex"	  },
			{ DD_BUFFERS_FLOAT,	 "a_tilingFactor" },
			{ DD_BUFFERS_UINT,	 "a_entityID"	  }
			});
		s_data.quadVertexArray->addVertexBuffer(s_data.quadVertexBuffer);

		s_data.QuadVertexBufferBase = new QuadVertex[s_data.maxVertices];

		uint32_t* quadIndices = new uint32_t[s_data.maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Shr_ptr<buffers::IndexBuffer> quadIndexBuff(buffers::IndexBuffer::create(quadIndices, s_data.maxIndices));
		s_data.quadVertexArray->setIndexBuffer(quadIndexBuff);
		delete[] quadIndices;

		s_data.whiteTexture = graphics::Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[s_data.maxTextureSlots];
		for (uint32_t i = 0; i < s_data.maxTextureSlots; i++)
			samplers[i] = i;

		//bool testBool;
		//std::string shaderPath = renderer2DUtils::get_default_shader_path(&testBool);
		auto[shaderPath, testBool] = utils::get_core_resource_file_location("shaders\\default2DShader.glsl");
		DD_CORE_ASSERT(testBool, "Default Shader file not found");
		s_data.defaultShader = Shader::create(shaderPath);
		
		s_data.textureSlots[0] = s_data.whiteTexture;

		s_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f };
		s_data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f };
		s_data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f };
		s_data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f };

		s_data.cameraUniformBuffer = buffers::UniformBuffer::create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::shutdown()
	{
		DD_PROFILE_FUNCTION();

		delete[] s_data.QuadVertexBufferBase;
	}

	void Renderer2D::begin(const OrthographicCamera& othoCamera) // TO DO: Remove
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT(!(s_data.beginCalled), "Renderer2D::end not called");
		s_data.beginCalled = true;

		s_data.defaultShader->bind();
		s_data.defaultShader->setUniformMat4(othoCamera.getProjectViewMatrix(), "u_projView");

		startBatch();
	}

	void Renderer2D::begin(const Camera& camera, const maths::Mat4& transform)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT(!(s_data.beginCalled), "Renderer2D::end not called");
		s_data.beginCalled = true;

		s_data.cameraBuffer.viewProjection = camera.getProjection() * maths::Mat4::invert(transform);
		s_data.cameraUniformBuffer->setData(&s_data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		startBatch();
	}

	void Renderer2D::begin(const graphics::EditorCamera& editorCamera)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT(!(s_data.beginCalled), "Renderer2D::end not called");
		s_data.beginCalled = true;

		s_data.cameraBuffer.viewProjection = editorCamera.getViewProjection();
		s_data.cameraUniformBuffer->setData(&s_data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		startBatch();
	}

	void Renderer2D::end()
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT((s_data.beginCalled), "Renderer2D::begin not called");
		s_data.beginCalled = false;

		flush();
	}

	void Renderer2D::flush()
	{
		DD_PROFILE_FUNCTION();

		//size of buffer in bytes
		uint32_t dataSize = (uint32_t)((uint8_t*)s_data.QuadVertexBufferPtr - (uint8_t*)s_data.QuadVertexBufferBase);
		s_data.quadVertexBuffer->setData(s_data.QuadVertexBufferBase, dataSize);

		// bind textures
		for (uint32_t i = 0; i < s_data.textureSlotIndex; i++)
			s_data.textureSlots[i]->bind(i);

		s_data.defaultShader->bind();

		// if there is nothing to render skip this
		if (dataSize != 0)
		{
			RenderCommands::drawIndexed(s_data.quadVertexArray, s_data.quadIndexCount);
#ifndef DD_DISTRO
			s_data.stats.drawCalls++;
#endif 
		}
	}

	void Renderer2D::startBatch()
	{
		s_data.quadIndexCount = 0;
		s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;

		s_data.textureSlotIndex = 1;
	}

	void Renderer2D::flushAndReset()
	{
		flush();
		startBatch();
	}

	void Renderer2D::drawQuad(const primatives2D::QuadProperties& quadProps, uint32_t entityID)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT((s_data.beginCalled), "Renderer2D::begin not called");

		// TO DO: flush and reset if all texture slots are taken

		//flush and reset if all the indices are used
		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		maths::Vec2 texCoords[4] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		float textureIndex = 0.0f;
		if (quadProps.texture) //might need to also check if the shr_ptr is still valid
		{
			for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
			{
				if (*s_data.textureSlots[i].get() == *quadProps.texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_data.textureSlotIndex;
				s_data.textureSlots[s_data.textureSlotIndex] = quadProps.texture;
				s_data.textureSlotIndex++;
			}
		}
		else if (quadProps.subTexture)
		{
			for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
			{
				if (*s_data.textureSlots[i].get() == *quadProps.subTexture->getTexture().get())
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_data.textureSlotIndex;
				s_data.textureSlots[s_data.textureSlotIndex] = quadProps.subTexture->getTexture();
				s_data.textureSlotIndex++;
			}

			auto subTexCoords = quadProps.subTexture->getTextureCoords();
			texCoords[0] = subTexCoords[0];
			texCoords[1] = subTexCoords[1];
			texCoords[2] = subTexCoords[2];
			texCoords[3] = subTexCoords[3];
		}

		/*maths::Mat4 transform = maths::Mat4::translate(quadProps.position)
			* maths::Mat4::scale({ quadProps.size.x, quadProps.size.y, 1 });*/

		s_data.QuadVertexBufferPtr->position = quadProps.transform * s_data.quadVertexPositions[0];
		s_data.QuadVertexBufferPtr->colour = quadProps.colour;
		s_data.QuadVertexBufferPtr->texCoord = texCoords[0];
		s_data.QuadVertexBufferPtr->texIndex = textureIndex;
		s_data.QuadVertexBufferPtr->tilingFactor = quadProps.tilingFactor;
		s_data.QuadVertexBufferPtr->entityID = entityID;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = quadProps.transform * s_data.quadVertexPositions[1];
		s_data.QuadVertexBufferPtr->colour = quadProps.colour;
		s_data.QuadVertexBufferPtr->texCoord = texCoords[1];
		s_data.QuadVertexBufferPtr->texIndex = textureIndex;
		s_data.QuadVertexBufferPtr->tilingFactor = quadProps.tilingFactor;
		s_data.QuadVertexBufferPtr->entityID = entityID;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = quadProps.transform * s_data.quadVertexPositions[2];
		s_data.QuadVertexBufferPtr->colour = quadProps.colour;
		s_data.QuadVertexBufferPtr->texCoord = texCoords[2];
		s_data.QuadVertexBufferPtr->texIndex = textureIndex;
		s_data.QuadVertexBufferPtr->tilingFactor = quadProps.tilingFactor;
		s_data.QuadVertexBufferPtr->entityID = entityID;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = quadProps.transform * s_data.quadVertexPositions[3];
		s_data.QuadVertexBufferPtr->colour = quadProps.colour;
		s_data.QuadVertexBufferPtr->texCoord = texCoords[3];
		s_data.QuadVertexBufferPtr->texIndex = textureIndex;
		s_data.QuadVertexBufferPtr->tilingFactor = quadProps.tilingFactor;
		s_data.QuadVertexBufferPtr->entityID = entityID;
		s_data.QuadVertexBufferPtr++;

		s_data.quadIndexCount += 6;

#ifndef DD_DISTRO
		s_data.stats.quadCount++;
#endif 
	}

	void Renderer2D::drawRotatedQuad(const primatives2D::RotatedQuadProperties& rotQuadProps, uint32_t entityID)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT((s_data.beginCalled), "Renderer2D::begin not called");

		// TO DO: flush if the textureSlots are all full

		//flush and reset if all the indices are used
		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		maths::Vec2 texCoords[4] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		float textureIndex = 0.0f;
		if (rotQuadProps.texture) //might need to also check if the shr_ptr is still valid
		{
			for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
			{
				if (*s_data.textureSlots[i].get() == *rotQuadProps.texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_data.textureSlotIndex;
				s_data.textureSlots[s_data.textureSlotIndex] = rotQuadProps.texture;
				s_data.textureSlotIndex++;
			}
		}
		else if (rotQuadProps.subTexture)
		{
			for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
			{
				if (*s_data.textureSlots[i].get() == *rotQuadProps.subTexture->getTexture().get())
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_data.textureSlotIndex;
				s_data.textureSlots[s_data.textureSlotIndex] = rotQuadProps.subTexture->getTexture();
				s_data.textureSlotIndex++;
			}

			auto subTexCoords = rotQuadProps.subTexture->getTextureCoords();
			texCoords[0] = subTexCoords[0];
			texCoords[1] = subTexCoords[1];
			texCoords[2] = subTexCoords[2];
			texCoords[3] = subTexCoords[3];
		}

		/*maths::Mat4 transform = maths::Mat4::translate(rotQuadProps.position)
			* maths::Mat4::rotate(rotQuadProps.rotation, { 0,0,1 }, true)
			* maths::Mat4::scale({ rotQuadProps.size.x, rotQuadProps.size.y, 1 });*/

		s_data.QuadVertexBufferPtr->position = rotQuadProps.transform * s_data.quadVertexPositions[0];
		s_data.QuadVertexBufferPtr->texCoord = texCoords[0];
		s_data.QuadVertexBufferPtr->texIndex = textureIndex;
		s_data.QuadVertexBufferPtr->tilingFactor = rotQuadProps.tilingFactor;
		s_data.QuadVertexBufferPtr->colour = rotQuadProps.colour;
		s_data.QuadVertexBufferPtr->entityID = entityID;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = rotQuadProps.transform * s_data.quadVertexPositions[1];
		s_data.QuadVertexBufferPtr->texCoord = texCoords[1];
		s_data.QuadVertexBufferPtr->texIndex = textureIndex;
		s_data.QuadVertexBufferPtr->tilingFactor = rotQuadProps.tilingFactor;
		s_data.QuadVertexBufferPtr->colour = rotQuadProps.colour;
		s_data.QuadVertexBufferPtr->entityID = entityID;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = rotQuadProps.transform * s_data.quadVertexPositions[2];
		s_data.QuadVertexBufferPtr->texCoord = texCoords[2];
		s_data.QuadVertexBufferPtr->texIndex = textureIndex;
		s_data.QuadVertexBufferPtr->tilingFactor = rotQuadProps.tilingFactor;
		s_data.QuadVertexBufferPtr->colour = rotQuadProps.colour;
		s_data.QuadVertexBufferPtr->entityID = entityID;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = rotQuadProps.transform * s_data.quadVertexPositions[3];
		s_data.QuadVertexBufferPtr->texCoord = texCoords[3];
		s_data.QuadVertexBufferPtr->texIndex = textureIndex;
		s_data.QuadVertexBufferPtr->tilingFactor = rotQuadProps.tilingFactor;
		s_data.QuadVertexBufferPtr->colour = rotQuadProps.colour;
		s_data.QuadVertexBufferPtr->entityID = entityID;
		s_data.QuadVertexBufferPtr++;

		s_data.quadIndexCount += 6;

#ifndef DD_DISTRO
		s_data.stats.quadCount++;
#endif 
	}

	void Renderer2D::drawSprite(const maths::Mat4& transform, scene::SpriteRendererComponent& spriteComponent, uint32_t entityID)
	{
		// QuadProperties stores potentially alot of wasted/unwanted data
		// might make this function do that same as drawQuad just with all the data from spriteComponent

		drawQuad({ transform, spriteComponent.material.texture, spriteComponent.material.tilingFactor, spriteComponent.colour }, entityID);
	}

#ifndef DD_DISTRO
	void Renderer2D::resetStats()
	{
		memset(&s_data.stats, 0, sizeof(Renderer2D::Statistics));
	}

	Renderer2D::Statistics Renderer2D::getStats()
	{
		return s_data.stats;
	}
#endif

} }