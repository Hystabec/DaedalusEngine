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
		static void drawQuad(const primatives2D::QuadProperties& quadProps);

		//static void drawRotatedQuad(const primatives2D::RotatedQuadProperties& rotQuadProps); 
		//-potential implementation of rotated quads - might want to keep seperate from standard quads for collision detection (AABB) 
		// as wells as standard quad wont need to do mat4::rotate for a non rotated quad.
	};

} }