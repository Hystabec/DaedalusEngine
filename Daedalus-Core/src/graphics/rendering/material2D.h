#pragma once

#include "texture.h"
#include "core.h"

namespace daedalus::graphics {

	// This can be expanded once more is required of it
	struct Material2D
	{
		AssetHandle texture = 0;
		float tilingFactor = 1.0f;

		Material2D() = default;
		Material2D(AssetHandle texture)
			: texture(texture)
		{
		}
	};

}