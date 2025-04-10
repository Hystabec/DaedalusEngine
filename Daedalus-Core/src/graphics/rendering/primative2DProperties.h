#pragma once

#include "maths/maths.h"
#include "graphics/rendering/texture.h"

namespace daedalusCore { namespace graphics { namespace primatives2D {

	/// <summary>
	/// Struct used to store the properties of Quads used in renderer2D::drawQuad
	/// </summary>
	struct QuadProperties
	{
		QuadProperties() 
		{
		};

		QuadProperties(const maths::vec2& position)
			: position({ position.x, position.y, 0.0f })
		{
		}

		QuadProperties(const maths::vec3& position)
			: position(position)
		{
		}

		QuadProperties(const maths::vec2& position, const maths::vec2& size)
			: position({ position.x, position.y, 0.0f }), size(size)
		{
		}

		QuadProperties(const maths::vec3& position, const maths::vec2& size)
			: position(position), size(size)
		{
		}

		QuadProperties(const maths::vec2& position, const maths::vec2& size, const maths::vec4& colourTint)
			: position({ position.x, position.y, 0.0f }), size(size), colour(colourTint)
		{
		}

		QuadProperties(const maths::vec3& position, const maths::vec2& size, const maths::vec4& colourTint)
			: position(position), size(size), colour(colourTint)
		{
		}

		QuadProperties(const maths::vec3& position, const maths::vec2& size, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: position(position), size(size), texture(texture), colour(colourTint)
		{
		}

		QuadProperties(const maths::vec2& position, const maths::vec2& size, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: position({ position.x, position.y, 0.0f }), size(size), texture(texture), colour(colourTint)
		{
		}

		QuadProperties(const maths::vec3& position, const maths::vec2& size, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: position(position), size(size), texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
		}

		QuadProperties(const maths::vec2& position, const maths::vec2& size, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: position({ position.x, position.y, 0.0f }), size(size), texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
		}

		maths::vec3 position = { 0.0f };
		maths::vec2 size = { 1.0f };
		shr_ptr<Texture2D> texture = nullptr;
		float tilingFactor = 1.0f;
		maths::vec4 colour = { 1.0f };
	};

	/// <summary>
	/// Struct used to store the properties of RotatedQuads used in renderer2D::drawRotatedQuad
	/// </summary>
	struct RotatedQuadProperties
	{
		RotatedQuadProperties()
		{
		};

		/*
		RotatedQuadProperties(const QuadProperties& quadProps, const float& rotation)
			: position(quadProps.position), size(quadProps.size), rotation(rotation), texture(quadProps.texture), tilingFactor(quadProps.tilingFactor), colour(quadProps.colour)
		{
		}
		*/

		RotatedQuadProperties(const maths::vec2& position)
			: position({ position.x, position.y, 0.0f })
		{
		}

		RotatedQuadProperties(const maths::vec3& position)
			: position(position)
		{
		}

		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size)
			: position({ position.x, position.y, 0.0f }), size(size)
		{
		}

		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size)
			: position(position), size(size)
		{
		}

		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation)
			: position({ position.x, position.y, 0.0f }), size(size), rotation(rotation)
		{
		}

		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation)
			: position(position), size(size), rotation(rotation)
		{
		}

		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const maths::vec4& colourTint)
			: position({ position.x, position.y, 0.0f }), size(size), rotation(rotation), colour(colourTint)
		{
		}

		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: position(position), size(size), rotation(rotation), texture(texture), colour(colourTint)
		{
		}

		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: position({ position.x, position.y, 0.0f }), size(size), rotation(rotation), texture(texture), colour(colourTint)
		{
		}

		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const maths::vec4& colourTint)
			: position(position), size(size), rotation(rotation), colour(colourTint)
		{
		}

		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: position({ position.x, position.y, 0.0f }), size(size), rotation(rotation), texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
		}

		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: position(position), size(size), rotation(rotation), texture(texture), tilingFactor(tilingFactor), colour(colourTint)
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