#pragma once

#include "graphics/camera/orthographicCamera.h"
#include "graphics/rendering/texture.h"

namespace daedalusCore { namespace graphics {

	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void begin(const OrthographicCamera& othoCamera);
		static void end();

		// primatives
		static void drawQuad(const maths::vec2& position, const maths::vec2& size, const float& rotation, const maths::vec4& colour);
		static void drawQuad(const maths::vec3& position, const maths::vec2& size, const float& rotation, const maths::vec4& colour);
		static void drawQuad(const maths::vec2& position, const maths::vec2& size, const float& rotation, const shr_ptr<graphics::Texture2D>& texture, const maths::vec4& colour = { 1 });
		static void drawQuad(const maths::vec3& position, const maths::vec2& size, const float& rotation, const shr_ptr<graphics::Texture2D>& texture, const maths::vec4& colour = { 1 });
	};

} }