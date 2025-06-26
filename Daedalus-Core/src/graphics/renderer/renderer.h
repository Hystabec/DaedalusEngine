#pragma once
#include "renderCommands.h"

#include "graphics/camera/orthographicCamera.h"
#include "graphics/shaders/shader.h"

namespace daedalus { namespace graphics {

	class Renderer
	{
	public:
		static void init();
		static void shutdown();

		static void onWindowResize(uint32_t width, uint32_t height);

		static void begin(OrthographicCamera& othoCamera);
		static void end();

		static void submit(const Shr_ptr<graphics::buffers::VertexArray>& vertexArray, const Shr_ptr<graphics::Shader>& shader, const maths::Mat4& transform = maths::Mat4(1.0f));

		static inline RendererAPI::API getCurrentAPI() { return RendererAPI::getAPI(); }

	private:
		struct sceneData
		{
			bool inUse = false;
			maths::Mat4 projectionViewMatrix;
		};

		static sceneData* m_sceneData;
	};

} }