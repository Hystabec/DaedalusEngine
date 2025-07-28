#pragma once

#include "texture.h"
#include "core.h"

namespace daedalus::graphics {

	// This can be expanded once more is required of it
	struct Material2D
	{
		Shr_ptr<Texture2D> texture;
		float tilingFactor = 1.0f;

		Material2D() = default;
		Material2D(Shr_ptr<Texture2D> texture)
			: texture(texture)
		{
		}
	};

}