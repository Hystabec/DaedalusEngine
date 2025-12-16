#pragma once

#include "asset.h"

#include <map>

namespace daedalus {

	// NOTE: This could potentialy be made into an unordered_map (and probably should)
	using AssetMap = std::map<AssetHandle, Shr_ptr<Asset>>;

	class AssetManagerBase
	{
	public:
		virtual bool isAssetHandleValid(AssetHandle handle) const = 0;

		virtual Shr_ptr<Asset> getAsset(AssetHandle handle) = 0;
		virtual AssetType getAssetType(AssetHandle handle) const = 0;
		virtual bool isAssetLoaded(AssetHandle handle) const = 0;
	};

}