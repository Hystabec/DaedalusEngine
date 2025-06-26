#pragma once

#include "graphics/camera/camera.h"
#include "graphics/camera/orthographicCamera.h"
#include "graphics/rendering/texture.h"
#include "graphics/rendering/primative2DProperties.h"

namespace daedalus { namespace graphics {

	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void begin(const Camera& camera, const maths::mat4& transform);
		static void begin(const OrthographicCamera& othoCamera); // TO DO: Remove
		static void end();
		static void flush();


		// primatives
		
		/// @brief Draws a 2D Quad to the screen
		static void drawQuad(const primatives2D::QuadProperties& quadProps);
		
		/// @brief  Draws a 2D Quad to the screen with the option to be rotated.
		/// Rotation should be in radians
		static void drawRotatedQuad(const primatives2D::RotatedQuadProperties& rotQuadProps); 

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
		static void flushAndReset();
	};

} }