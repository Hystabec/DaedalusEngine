#pragma once

#include "asset.h"
#include "../core.h"

#include <map>

namespace daedalus {

	using AssetMap = std::map<AssetHandle, Shr_ptr<Asset>>;

	class AssetManagerBase
	{
	public:
		virtual Shr_ptr<Asset> getAsset(AssetHandle handle) const = 0;

		virtual bool isAssetHandleValid(AssetHandle handle) const = 0;
		virtual bool isAssetLoaded(AssetHandle handle) const = 0;
	};

}