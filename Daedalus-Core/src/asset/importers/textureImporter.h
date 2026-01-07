#pragma once

#include "../asset.h"
#include "../assetMetadata.h"

#include "../../graphics/rendering/texture.h"

namespace daedalus {

	class TextureImporter
	{
	public:
		static IntrusivePtr<graphics::Texture2D> importTexture2D(AssetHandle handle, const AssetMetadata& metadata);

		// Path has to be relative or absolute
		static IntrusivePtr<graphics::Texture2D> loadTexture2D(const std::filesystem::path& path);
	};

}