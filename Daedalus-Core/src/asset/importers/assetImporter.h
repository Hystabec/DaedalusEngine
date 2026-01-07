#pragma once

#include "../assetMetadata.h"

namespace daedalus {

	class AssetImporter
	{
	public:
		static IntrusivePtr<Asset> importAsset(AssetHandle handle, const AssetMetadata& metadata);
	};

}