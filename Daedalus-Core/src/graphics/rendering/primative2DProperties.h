#pragma once

#include "maths/maths.h"
#include "graphics/rendering/texture.h"

namespace daedalusCore { namespace graphics { namespace primatives2D {

	struct QuadProperties
	{
		QuadProperties() 
		{
		};

		QuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const maths::vec4& colourTint)
			: position({ position.x, position.y, 0.0f }), size(size), rotation(rotation), colour(colourTint)
		{
		}

		QuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const maths::vec4& colourTint)
			: position(position), size(size), rotation(rotation), colour(colourTint)
		{
		}

		QuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: position(position), size(size), rotation(rotation), texture(texture), colour(colourTint)
		{
		}

		QuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: position({ position.x, position.y, 0.0f }), size(size), rotation(rotation), texture(texture), colour(colourTint)
		{
		}

		QuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: position(position), size(size), rotation(rotation), texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
		}

		QuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: position({ position.x, position.y, 0.0f }), size(size), rotation(rotation), texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
		}

		maths::vec3 position = { 0.0f };
		maths::vec2 size = { 1.0f };
		float rotation = 0.0f;
		shr_ptr<Texture2D> texture = nullptr;
		float tilingFactor = 1.0f;
		maths::vec4 colour = { 1.0f };
	};

} } }