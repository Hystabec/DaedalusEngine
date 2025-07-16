#pragma once

#include "maths/maths.h"
#include "graphics/rendering/texture.h"
#include "graphics/rendering/subTexture2D.h"

namespace daedalus { namespace graphics { namespace primatives2D {

	/// @brief Struct used to store the properties of Quads used in renderer2D::drawQuad
	struct QuadProperties
	{
		QuadProperties() = default;

		QuadProperties(const maths::Vec3& position)
		{
			transform *= maths::Mat4::translate(position);
		}
		QuadProperties(const maths::Vec3& position, const maths::Vec2& size)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::Vec3& position, const maths::Vec2& size, const maths::Vec4& colourTint)
			: colour(colourTint)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::Vec3& position, const maths::Vec2& size, const Shr_ptr<Texture2D>& texture, const maths::Vec4& colourTint = { 1.0f })
			: texture(texture), colour(colourTint)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::Vec3& position, const maths::Vec2& size, const Shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::Vec4& colourTint = { 1.0f })
			: texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::Vec3& position, const maths::Vec2& size, const Shr_ptr<SubTexture2D>& subTexture, const maths::Vec4& colourTint = { 1.0f })
			: subTexture(subTexture), colour(colourTint)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		QuadProperties(const maths::Vec3& position, const maths::Vec2& size, const Shr_ptr<SubTexture2D>& subTexture, const float& tilingFactor, const maths::Vec4& colourTint = { 1.0f })
			: subTexture(subTexture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}

		QuadProperties(const maths::Mat4& matrix, const maths::Vec4& colour)
			: transform(matrix), colour(colour)
		{ }
		QuadProperties(const maths::Mat4& matrix, const Shr_ptr<Texture2D>& texture, const maths::Vec4& colourTint = { 1.0f })
			: transform(matrix), texture(texture), colour(colourTint)
		{ }
		QuadProperties(const maths::Mat4& matrix, const Shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::Vec4& colourTint = { 1.0f })
			: transform(matrix), texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{ }
		QuadProperties(const maths::Mat4& matrix, const Shr_ptr<SubTexture2D>& subTexture, const maths::Vec4& colourTint = { 1.0f })
			: transform(matrix), subTexture(subTexture), colour(colourTint)
		{ }
		QuadProperties(const maths::Mat4& matrix, const Shr_ptr<SubTexture2D>& subTexture, const float& tilingFactor, const maths::Vec4& colourTint = { 1.0f })
			: transform(matrix), subTexture(subTexture), tilingFactor(tilingFactor), colour(colourTint)
		{ }

		// As this is now storing the position and scale as a matrix and it can be constructed with a matrix,
		// there could be a rotation already in the matrix, so it could still be rotated anyway, you just cant
		// specify a rotation during the other construction options

		maths::Mat4 transform = maths::Mat4::identity();
		Shr_ptr<Texture2D> texture = nullptr;
		Shr_ptr<SubTexture2D> subTexture = nullptr;
		float tilingFactor = 1.0f;
		maths::Vec4 colour = { 1.0f };
	};
	
	/// @brief Struct used to store the properties of RotatedQuads used in renderer2D::drawRotatedQuad
	struct RotatedQuadProperties
	{
		RotatedQuadProperties() = default;
		
		RotatedQuadProperties(const maths::Vec3& position)
		{
			transform *= maths::Mat4::translate(position);
		}
		RotatedQuadProperties(const maths::Vec3& position, const maths::Vec2& size)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::Vec3& position, const maths::Vec2& size, const float& rotation)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::Vec3& position, const maths::Vec2& size, const float& rotation, const maths::Vec4& colour)
			: colour(colour)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::Vec3& position, const maths::Vec2& size, const float& rotation, const Shr_ptr<Texture2D>& texture, const maths::Vec4& colourTint = { 1.0f })
			: texture(texture), colour(colourTint)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::Vec3& position, const maths::Vec2& size, const float& rotation, const Shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::Vec4& colourTint = { 1.0f })
			: texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::Vec3& position, const maths::Vec2& size, const float& rotation, const Shr_ptr<SubTexture2D>& subTexture, const maths::Vec4& colourTint = { 1.0f })
			: subTexture(subTexture), colour(colourTint)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}
		RotatedQuadProperties(const maths::Vec3& position, const maths::Vec2& size, const float& rotation, const Shr_ptr<SubTexture2D>& subTexture, const float& tilingFactor, const maths::Vec4& colourTint = { 1.0f })
			: subTexture(subTexture), tilingFactor(tilingFactor), colour(colourTint)
		{
			transform *= maths::Mat4::translate(position);
			transform *= maths::Mat4::rotate(rotation, { 0,0,1 }, true);
			transform *= maths::Mat4::scale({ size.x, size.y, 1.0f });
		}

		RotatedQuadProperties(const maths::Mat4& matrix, const maths::Vec4& colour)
			: transform(matrix), colour(colour)
		{
		}
		RotatedQuadProperties(const maths::Mat4& matrix, const Shr_ptr<Texture2D>& texture, const maths::Vec4& colourTint = { 1.0f })
			: transform(matrix), texture(texture), colour(colourTint)
		{
		}
		RotatedQuadProperties(const maths::Mat4& matrix, const Shr_ptr<Texture2D>& texture, const float& tilingFactor, const maths::Vec4& colourTint = { 1.0f })
			: transform(matrix), texture(texture), tilingFactor(tilingFactor), colour(colourTint)
		{
		}
		RotatedQuadProperties(const maths::Mat4& matrix, const Shr_ptr<SubTexture2D>& subTexture, const maths::Vec4& colourTint = { 1.0f })
			: transform(matrix), subTexture(subTexture), colour(colourTint)
		{
		}
		RotatedQuadProperties(const maths::Mat4& matrix, const Shr_ptr<SubTexture2D>& subTexture, const float& tilingFactor, const maths::Vec4& colourTint = { 1.0f })
			: transform(matrix), subTexture(subTexture), tilingFactor(tilingFactor), colour(colourTint)
		{
		}

		maths::Mat4 transform = maths::Mat4::identity();
		Shr_ptr<Texture2D> texture = nullptr;
		Shr_ptr<SubTexture2D> subTexture = nullptr;
		float tilingFactor = 1.0f;
		maths::Vec4 colour = { 1.0f };
	};

} } }