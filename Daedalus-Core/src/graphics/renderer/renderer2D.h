#pragma once

#include "graphics/camera/orthographicCamera.h"

namespace daedalusCore { namespace graphics {

	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void begin(const OrthographicCamera& othoCamera);
		static void end();

		// primatives
		static void drawQuad(const maths::vec2& position, const maths::vec2& size, const maths::vec4& colour);
		static void drawQuad(const maths::vec2& position, const maths::vec2& size, const float& rotation, const maths::vec4& colour);
		static void drawQuad(const maths::vec3& position, const maths::vec2& size, const maths::vec4& colour);
		static void drawQuad(const maths::vec3& position, const maths::vec2& size, const float& rotation, const maths::vec4& colour);
	};

} }