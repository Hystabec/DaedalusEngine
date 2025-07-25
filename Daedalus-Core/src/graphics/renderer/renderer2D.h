#pragma once

#include "graphics/camera/camera.h"
#include "graphics/camera/orthographicCamera.h"
#include "graphics/camera/editorCamera.h"

#include "graphics/rendering/texture.h"
#include "graphics/rendering/primative2DProperties.h"
#include "scene/entityComponents/spriteRendererComponent.h"

namespace daedalus { namespace graphics {

	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void begin(const OrthographicCamera& othoCamera); // TO DO: Remove
		static void begin(const Camera& camera, const maths::Mat4& transform);
		static void begin(const graphics::EditorCamera& editorCamera);
		static void end();
		static void flush();


		// primatives
		
		/// @brief Draws a 2D Quad to the screen
		/// @brief - Default entityID is UINT32_MAX as that is the same as entt::null
		static void drawQuad(const primatives2D::QuadProperties& quadProps, uint32_t entityID = UINT32_MAX);
		
		/// @brief Draws a 2D Quad to the screen with the option to be rotated.
		/// @brief Rotation should be in radians
		/// @brief - Default entityID is UINT32_MAX as that is the same as entt::null
		static void drawRotatedQuad(const primatives2D::RotatedQuadProperties& rotQuadProps, uint32_t entityID = UINT32_MAX);

		static void drawSprite(const maths::Mat4& transform, scene::SpriteRendererComponent& spriteComponent, uint32_t entityID);

#ifndef DD_DISTRO
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;

			uint32_t getTotalVertexCount() { return quadCount * 4; }
			uint32_t getTotalIndexCount() { return quadCount * 6; }
		};
		static void resetStats();
		static Statistics getStats();
#endif
	private:
		static void startBatch();
		static void flushAndReset();
	};

} }