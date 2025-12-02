#pragma once

#include "assetMetadata.h"

namespace daedalus {

	class AssetImporter
	{
	public:
		static Shr_ptr<Asset> importAsset(AssetHandle handle, const AssetMetadata& metadata);
	};

}