#pragma once
#include "renderCommands.h"

#include "graphics/camera/orthographicCamera.h"
#include "graphics/shader.h"

namespace daedalusCore { namespace graphics {

	class Renderer
	{
	public:
		static void init();

		static void begin(OrthographicCamera& othoCamera);
		static void end();

		static void submit(const shr_ptr<graphics::buffers::VertexArray>& vertexArray, const shr_ptr<graphics::Shader>& shader, const maths::mat4& transform = maths::mat4(1.0f));

		static inline RendererAPI::API getCurrentAPI() { return RendererAPI::getAPI(); }

	private:
		struct sceneData
		{
			bool inUse = false;
			maths::mat4 projectionViewMatrix;
		};

		static sceneData* m_sceneData;
	};

} }