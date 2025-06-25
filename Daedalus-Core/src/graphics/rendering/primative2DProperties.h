#pragma once

#include "maths/maths.h"
#include "graphics/rendering/texture.h"
#include "graphics/rendering/subTexture2D.h"

namespace daedalus { namespace graphics { namespace primatives2D {

	/// @brief Struct used to store the properties of Quads used in renderer2D::drawQuad
	struct QuadProperties
	{
		QuadProperties() = default;

		QuadProperties(const maths::vec2& position)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
		}
		QuadProperties(const maths::vec2& position, const maths::vec2& size)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::vec2& position, const maths::vec2& size, const maths::vec4& colourTint)
			: colour(colourTint)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::vec2& position, const maths::vec2& size, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: texture(texture), colour(colourTint)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::vec2& position, const maths::vec2& size, const shr_ptr<SubTexture2D>& subTexture, const maths::vec4& colourTint = { 1.0f })
			: subTexture(subTexture), colour(colourTint)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::vec2& position, const maths::vec2& size, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::vec2& position, const maths::vec2& size, const shr_ptr<SubTexture2D>& subTexture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: subTexture(subTexture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::mat4::translate({position.x, position.y, 0.0f});
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}

		QuadProperties(const maths::vec3& position)
		{
			transform *= maths::mat4::translate(position);
		}
		QuadProperties(const maths::vec3& position, const maths::vec2& size)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::vec3& position, const maths::vec2& size, const maths::vec4& colourTint)
			: colour(colourTint)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::vec3& position, const maths::vec2& size, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: texture(texture), colour(colourTint)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::vec3& position, const maths::vec2& size, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::vec3& position, const maths::vec2& size, const shr_ptr<SubTexture2D>& subTexture, const maths::vec4& colourTint = { 1.0f })
			: subTexture(subTexture), colour(colourTint)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::vec3& position, const maths::vec2& size, const shr_ptr<SubTexture2D>& subTexture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: subTexture(subTexture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}

		QuadProperties(const maths::mat4& matrix, const maths::vec4& colour)
			: transform(matrix), colour(colour)
		{ }
		QuadProperties(const maths::mat4& matrix, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: transform(matrix), texture(texture), colour(colourTint)
		{ }
		QuadProperties(const maths::mat4& matrix, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: transform(matrix), texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{ }
		QuadProperties(const maths::mat4& matrix, const shr_ptr<SubTexture2D>& subTexture, const maths::vec4& colourTint = { 1.0f })
			: transform(matrix), subTexture(subTexture), colour(colourTint)
		{ }
		QuadProperties(const maths::mat4& matrix, const shr_ptr<SubTexture2D>& subTexture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: transform(matrix), subTexture(subTexture), tilingFactor(tilingFactor), colour(colourTint)
		{ }

		// As this is now storing the position and scale as a matrix and it can be constructed with a matrix,
		// there could be a rotation already in the matrix, so it could still be rotated anyway, you just cant
		// specify a rotation during the other construction options

		maths::mat4 transform = maths::mat4::identity();
		shr_ptr<Texture2D> texture = nullptr;
		shr_ptr<SubTexture2D> subTexture = nullptr;
		float tilingFactor = 1.0f;
		maths::vec4 colour = { 1.0f };
	};
	
	/// @brief Struct used to store the properties of RotatedQuads used in renderer2D::drawRotatedQuad
	struct RotatedQuadProperties
	{
		RotatedQuadProperties() = default;

		RotatedQuadProperties(const maths::vec2& position)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
		}
		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const maths::vec4& colour)
			: colour(colour)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: texture(texture), colour(colourTint)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const shr_ptr<SubTexture2D>& subTexture, const maths::vec4& colourTint = { 1.0f })
			: subTexture(subTexture), colour(colourTint)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec2& position, const maths::vec2& size, const float& rotation, const shr_ptr<SubTexture2D>& subTexture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: subTexture(subTexture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::mat4::translate({ position.x, position.y, 0.0f });
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		
		RotatedQuadProperties(const maths::vec3& position)
		{
			transform *= maths::mat4::translate(position);
		}
		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const maths::vec4& colour)
			: colour(colour)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: texture(texture), colour(colourTint)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const shr_ptr<SubTexture2D>& subTexture, const maths::vec4& colourTint = { 1.0f })
			: subTexture(subTexture), colour(colourTint)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::vec3& position, const maths::vec2& size, const float& rotation, const shr_ptr<SubTexture2D>& subTexture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: subTexture(subTexture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::mat4::translate(position);
			transform *= maths::mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::mat4::scale({ size.x, size.y, 1.0f });
		}

		RotatedQuadProperties(const maths::mat4& matrix, const maths::vec4& colour)
			: transform(matrix), colour(colour)
		{
		}
		RotatedQuadProperties(const maths::mat4& matrix, const shr_ptr<Texture2D>& texture, const maths::vec4& colourTint = { 1.0f })
			: transform(matrix), texture(texture), colour(colourTint)
		{
		}
		RotatedQuadProperties(const maths::mat4& matrix, const shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: transform(matrix), texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
		}
		RotatedQuadProperties(const maths::mat4& matrix, const shr_ptr<SubTexture2D>& subTexture, const maths::vec4& colourTint = { 1.0f })
			: transform(matrix), subTexture(subTexture), colour(colourTint)
		{
		}
		RotatedQuadProperties(const maths::mat4& matrix, const shr_ptr<SubTexture2D>& subTexture, const float& tilingFactor, const maths::vec4& colourTint = { 1.0f })
			: transform(matrix), subTexture(subTexture), tilingFactor(tilingFactor), colour(colourTint)
		{
		}

		maths::mat4 transform = maths::mat4::identity();
		shr_ptr<Texture2D> texture = nullptr;
		shr_ptr<SubTexture2D> subTexture = nullptr;
		float tilingFactor = 1.0f;
		maths::vec4 colour = { 1.0f };
	};

} } }