#pragma once

#include "graphics/camera/orthographicCamera.h"
#include "graphics/rendering/texture.h"
#include "graphics/rendering/primative2DProperties.h"

namespace daedalusCore { namespace graphics {

	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void begin(const OrthographicCamera& othoCamera);
		static void end();

		// primatives

		/// <summary>
		/// Draws a 2D Quad to the screen
		/// </summary>
		static void drawQuad(const primatives2D::QuadProperties& quadProps);

		/// <summary>
		/// Draws a 2D Quad to the screen with the option to be rotated.
		/// Rotation should be in degrees
		/// </summary>
		static void drawRotatedQuad(const primatives2D::RotatedQuadProperties& rotQuadProps); 
	};

} }