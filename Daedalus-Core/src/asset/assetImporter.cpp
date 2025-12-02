#include "ddpch.h"
#include "assetImporter.h"

#include "textureImporter.h"

#include <map>

namespace daedalus {

	using AssetImporterFunction = std::function<Shr_ptr<Asset>(AssetHandle, const AssetMetadata&)>;
	static std::map<AssetType, AssetImporterFunction> s_assetImporterFunctions = {
		{ AssetType::Texture2D , TextureImporter::importTexture2D }
	};

	Shr_ptr<Asset> daedalus::AssetImporter::importAsset(AssetHandle handle, const AssetMetadata& metadata)
	{
		if (s_assetImporterFunctions.find(metadata.type) == s_assetImporterFunctions.end())
		{
			DD_CORE_LOG_ERROR("No importer available for asset type: {}", (uint16_t)metadata.type);
			return nullptr;
		}

		return s_assetImporterFunctions.at(metadata.type)(handle, metadata);
	}

}