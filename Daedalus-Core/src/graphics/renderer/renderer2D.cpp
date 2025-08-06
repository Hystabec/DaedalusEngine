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

	struct CircleVertex
	{
		maths::Vec3 worldPosition;
		maths::Vec3 localPosition;
		maths::Vec4 colour;
		float thickness;
		float fade;

		// Editor only
		uint32_t entityID;
	};

	struct LineVertex
	{
		maths::Vec3 position;
		maths::Vec4 colour;
	};

	struct Renderer2DData
	{
		// TO DO: Make maxQuads resizable as circles will be using this same value and if no circles/no quads are rendered
		// there is a lot of stale (unused) memory. So consider adding a resize function so the amount of memory can be set.
		// Could make it like a std::vector which increase the size by 50% when reaching the max size
		// To take it one step further when building into a game and all the objects are in the scene could set the max to that size
		// if resonable. (Might run into issues when spawning object through scripts as the would be unaccounted for)
		static const uint32_t maxQuads = 10000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32; // TO DO: RenderCaps

		bool beginCalled = false;

		// Quads
		Shr_ptr<buffers::VertexArray> quadVertexArray;
		Shr_ptr<buffers::VertexBuffer> quadVertexBuffer;
		Shr_ptr<Shader> defaultQuadShader;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		// Circles
		Shr_ptr<buffers::VertexArray> circleVertexArray;
		Shr_ptr<buffers::VertexBuffer> circleVertexBuffer;
		Shr_ptr<Shader> defaultCircleShader;

		uint32_t circleIndexCount = 0;
		CircleVertex* circleVertexBufferBase = nullptr;
		CircleVertex* circleVertexBufferPtr = nullptr;

		// Lines
		Shr_ptr<buffers::VertexArray> lineVertexArray;
		Shr_ptr<buffers::VertexBuffer> lineVertexBuffer;
		Shr_ptr<Shader> defaultLineShader;

		uint32_t lineVertexCount = 0;
		LineVertex* lineVertexBufferBase = nullptr;
		LineVertex* lineVertexBufferPtr = nullptr;

		float lineThickness = 2.0f;

		Shr_ptr<Texture2D> whiteTexture;
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

		// Quads
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

		s_data.quadVertexBufferBase = new QuadVertex[s_data.maxVertices];

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

		// Circles
		s_data.circleVertexArray = buffers::VertexArray::Create();

		s_data.circleVertexBuffer = buffers::VertexBuffer::create(s_data.maxVertices * sizeof(CircleVertex));
		s_data.circleVertexBuffer->setLayout({
			{ DD_BUFFERS_VEC3,	 "a_worldPosition"	},
			{ DD_BUFFERS_VEC3,	 "a_localPosition"	},
			{ DD_BUFFERS_VEC4,	 "a_colour"			},
			{ DD_BUFFERS_FLOAT,	 "a_Thickness"		},
			{ DD_BUFFERS_FLOAT,	 "a_Fade"			},
			{ DD_BUFFERS_UINT,	 "a_entityID"		}
			});
		s_data.circleVertexArray->addVertexBuffer(s_data.circleVertexBuffer);
		s_data.circleVertexArray->setIndexBuffer(quadIndexBuff); // Use quad IB - as the data is the same
		s_data.circleVertexBufferBase = new CircleVertex[s_data.maxVertices];

		// Lines
		s_data.lineVertexArray = buffers::VertexArray::Create();

		s_data.lineVertexBuffer = buffers::VertexBuffer::create(s_data.maxVertices * sizeof(LineVertex));
		s_data.lineVertexBuffer->setLayout({
			{ DD_BUFFERS_VEC3,	 "a_position"	},
			{ DD_BUFFERS_VEC4,	 "a_colour"			}
			});
		s_data.lineVertexArray->addVertexBuffer(s_data.lineVertexBuffer);
		s_data.lineVertexBufferBase = new LineVertex[s_data.maxVertices];

		// Textures
		s_data.whiteTexture = graphics::Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[s_data.maxTextureSlots];
		for (uint32_t i = 0; i < s_data.maxTextureSlots; i++)
			samplers[i] = i;

		// Shaders
		{
			auto [shaderPath, testBool] = utils::get_core_resource_file_location("shaders\\defaultQuad2DShader.glsl");
			DD_CORE_ASSERT(testBool, "Default quad shader file not found");
			s_data.defaultQuadShader = Shader::create(shaderPath);
		}
		{
			auto [shaderPath, testBool] = utils::get_core_resource_file_location("shaders\\defaultCircle2DShader.glsl");
			DD_CORE_ASSERT(testBool, "Default circle shader file not found");
			s_data.defaultCircleShader = Shader::create(shaderPath);
		}
		{
			auto [shaderPath, testBool] = utils::get_core_resource_file_location("shaders\\defaultLine2DShader.glsl");
			DD_CORE_ASSERT(testBool, "Default line shader file not found");
			s_data.defaultLineShader = Shader::create(shaderPath);
		}

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

		delete[] s_data.quadVertexBufferBase;
	}

	void Renderer2D::begin(const OrthographicCamera& othoCamera) // TO DO: Remove
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT(!(s_data.beginCalled), "Renderer2D::end not called");
		s_data.beginCalled = true;

		s_data.defaultQuadShader->bind();
		s_data.defaultQuadShader->setUniformMat4(othoCamera.getProjectViewMatrix(), "u_projView");

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
		flushQuads();
		flushCircles();
		flushLines();
	}

	void Renderer2D::startBatch()
	{
		DD_PROFILE_FUNCTION();
		startBatchQuads();
		startBatchCircles();
		startBatchLines();
	}

	void Renderer2D::startBatchQuads()
	{
		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
		s_data.textureSlotIndex = 1;
	}

	void Renderer2D::startBatchCircles()
	{
		s_data.circleIndexCount = 0;
		s_data.circleVertexBufferPtr = s_data.circleVertexBufferBase;
	}

	void Renderer2D::startBatchLines()
	{
		s_data.lineVertexCount = 0;
		s_data.lineVertexBufferPtr = s_data.lineVertexBufferBase;
	}

	void Renderer2D::flushQuads()
	{
		DD_PROFILE_FUNCTION();
		if (s_data.quadIndexCount)
		{
			//size of buffer in bytes
			uint32_t dataSize = (uint32_t)((uint8_t*)s_data.quadVertexBufferPtr - (uint8_t*)s_data.quadVertexBufferBase);
			s_data.quadVertexBuffer->setData(s_data.quadVertexBufferBase, dataSize);

			// bind textures
			for (uint32_t i = 0; i < s_data.textureSlotIndex; i++)
				s_data.textureSlots[i]->bind(i);

			s_data.defaultQuadShader->bind();

			RenderCommands::drawIndexed(s_data.quadVertexArray, s_data.quadIndexCount);
#ifndef DD_DISTRO
			s_data.stats.drawCalls++;
#endif 
		}
	}

	void Renderer2D::flushCircles()
	{
		DD_PROFILE_FUNCTION();
		if (s_data.circleIndexCount)
		{
			//size of buffer in bytes
			uint32_t dataSize = (uint32_t)((uint8_t*)s_data.circleVertexBufferPtr - (uint8_t*)s_data.circleVertexBufferBase);
			s_data.circleVertexBuffer->setData(s_data.circleVertexBufferBase, dataSize);

			s_data.defaultCircleShader->bind();
			RenderCommands::drawIndexed(s_data.circleVertexArray, s_data.circleIndexCount);
#ifndef DD_DISTRO
			s_data.stats.drawCalls++;
#endif 
		}
	}

	void Renderer2D::flushLines()
	{
		DD_PROFILE_FUNCTION();
		if (s_data.lineVertexCount)
		{
			//size of buffer in bytes
			uint32_t dataSize = (uint32_t)((uint8_t*)s_data.lineVertexBufferPtr - (uint8_t*)s_data.lineVertexBufferBase);
			s_data.lineVertexBuffer->setData(s_data.lineVertexBufferBase, dataSize);

			s_data.defaultLineShader->bind();
			RenderCommands::setLineThickness(s_data.lineThickness);
			RenderCommands::drawLines(s_data.lineVertexArray, s_data.lineVertexCount);
#ifndef DD_DISTRO
			s_data.stats.drawCalls++;
#endif 
		}
	}

	void Renderer2D::flushAndResetQuads()
	{
		flushQuads();
		startBatchQuads();
	}

	void Renderer2D::flushAndResetCircles()
	{
		flushCircles();
		startBatchCircles();
	}

	void Renderer2D::flushAndResetLines()
	{
		flushLines();
		startBatchLines();
	}

	void Renderer2D::drawQuad(const primatives2D::QuadProperties& quadProps, uint32_t entityID)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT((s_data.beginCalled), "Renderer2D::begin not called");

		// TO DO: flush and reset if all texture slots are taken

		//flush and reset if all the indices are used
		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndResetQuads();

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

		s_data.quadVertexBufferPtr->position = quadProps.transform * s_data.quadVertexPositions[0];
		s_data.quadVertexBufferPtr->colour = quadProps.colour;
		s_data.quadVertexBufferPtr->texCoord = texCoords[0];
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = quadProps.tilingFactor;
		s_data.quadVertexBufferPtr->entityID = entityID;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = quadProps.transform * s_data.quadVertexPositions[1];
		s_data.quadVertexBufferPtr->colour = quadProps.colour;
		s_data.quadVertexBufferPtr->texCoord = texCoords[1];
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = quadProps.tilingFactor;
		s_data.quadVertexBufferPtr->entityID = entityID;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = quadProps.transform * s_data.quadVertexPositions[2];
		s_data.quadVertexBufferPtr->colour = quadProps.colour;
		s_data.quadVertexBufferPtr->texCoord = texCoords[2];
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = quadProps.tilingFactor;
		s_data.quadVertexBufferPtr->entityID = entityID;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = quadProps.transform * s_data.quadVertexPositions[3];
		s_data.quadVertexBufferPtr->colour = quadProps.colour;
		s_data.quadVertexBufferPtr->texCoord = texCoords[3];
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = quadProps.tilingFactor;
		s_data.quadVertexBufferPtr->entityID = entityID;
		s_data.quadVertexBufferPtr++;

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
			flushAndResetQuads();

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

		s_data.quadVertexBufferPtr->position = rotQuadProps.transform * s_data.quadVertexPositions[0];
		s_data.quadVertexBufferPtr->texCoord = texCoords[0];
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = rotQuadProps.tilingFactor;
		s_data.quadVertexBufferPtr->colour = rotQuadProps.colour;
		s_data.quadVertexBufferPtr->entityID = entityID;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = rotQuadProps.transform * s_data.quadVertexPositions[1];
		s_data.quadVertexBufferPtr->texCoord = texCoords[1];
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = rotQuadProps.tilingFactor;
		s_data.quadVertexBufferPtr->colour = rotQuadProps.colour;
		s_data.quadVertexBufferPtr->entityID = entityID;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = rotQuadProps.transform * s_data.quadVertexPositions[2];
		s_data.quadVertexBufferPtr->texCoord = texCoords[2];
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = rotQuadProps.tilingFactor;
		s_data.quadVertexBufferPtr->colour = rotQuadProps.colour;
		s_data.quadVertexBufferPtr->entityID = entityID;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = rotQuadProps.transform * s_data.quadVertexPositions[3];
		s_data.quadVertexBufferPtr->texCoord = texCoords[3];
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = rotQuadProps.tilingFactor;
		s_data.quadVertexBufferPtr->colour = rotQuadProps.colour;
		s_data.quadVertexBufferPtr->entityID = entityID;
		s_data.quadVertexBufferPtr++;

		s_data.quadIndexCount += 6;

#ifndef DD_DISTRO
		s_data.stats.quadCount++;
#endif 
	}

	void Renderer2D::drawCircle(const maths::Mat4& transform, const maths::Vec4& colour, float thickness, float fade, uint32_t entityID)
	{
		DD_PROFILE_FUNCTION();
		DD_CORE_ASSERT((s_data.beginCalled), "Renderer2D::begin not called");

		//flush and reset if all the indices are used
		if (s_data.circleIndexCount >= Renderer2DData::maxIndices)
			flushAndResetCircles();

		for (uint32_t i = 0; i < 4; i++)
		{
			s_data.circleVertexBufferPtr->worldPosition = transform * s_data.quadVertexPositions[i];
			s_data.circleVertexBufferPtr->localPosition = s_data.quadVertexPositions[i] * 2.0f;
			s_data.circleVertexBufferPtr->colour = colour;
			s_data.circleVertexBufferPtr->thickness = thickness;
			s_data.circleVertexBufferPtr->fade = fade;
			s_data.circleVertexBufferPtr->entityID = entityID;
			s_data.circleVertexBufferPtr++;
		}

		s_data.circleIndexCount += 6;

#ifndef DD_DISTRO
		// could split this up into circleCount - but they are tecnically quads
		s_data.stats.quadCount++;
#endif 
	}

	void Renderer2D::drawLine(const maths::Vec3& p0, const maths::Vec3& p1, const maths::Vec4& colour)
	{
		s_data.lineVertexBufferPtr->position = p0;
		s_data.lineVertexBufferPtr->colour = colour;
		s_data.lineVertexBufferPtr++;

		s_data.lineVertexBufferPtr->position = p1;
		s_data.lineVertexBufferPtr->colour = colour;
		s_data.lineVertexBufferPtr++;

		s_data.lineVertexCount += 2;
	}

	void Renderer2D::drawRect(const maths::Vec3& position, const maths::Vec2& size, const maths::Vec4& colour)
	{
		maths::Vec3 p0 = maths::Vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		maths::Vec3 p1 = maths::Vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		maths::Vec3 p2 = maths::Vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		maths::Vec3 p3 = maths::Vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		drawLine(p0, p1, colour);
		drawLine(p1, p2, colour);
		drawLine(p2, p3, colour);
		drawLine(p3, p0, colour);
	}

	void Renderer2D::drawRect(const maths::Mat4& transform, const maths::Vec4& colour)
	{
		maths::Vec3 lineVertices[4];
		for (uint32_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_data.quadVertexPositions[i];

		drawLine(lineVertices[0], lineVertices[1], colour);
		drawLine(lineVertices[1], lineVertices[2], colour);
		drawLine(lineVertices[2], lineVertices[3], colour);
		drawLine(lineVertices[3], lineVertices[0], colour);
	}

	float Renderer2D::getLineThickness()
	{
		return s_data.lineThickness;
	}

	void Renderer2D::setLineThickness(float thickness)
	{
		s_data.lineThickness = thickness;
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