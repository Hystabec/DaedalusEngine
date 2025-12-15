#pragma once

#include "assetManagerBase.h"
#include "../project/project.h"

namespace daedalus {

	class AssetManager
	{
	public:
		static bool isAssetHandleValid(AssetHandle handle)
		{
			return Project::getActive()->getAssetManager()->isAssetHandleValid(handle);
		}

		template<typename T>
		static Shr_ptr<T> getAsset(AssetHandle handle)
		{
			Shr_ptr<Asset> asset = Project::getActive()->getAssetManager()->getAsset(handle);
			return std::static_pointer_cast<T>(asset);
		}

		static AssetType getAssetType(AssetHandle handle)
		{
			return Project::getActive()->getAssetManager()->getAssetType(handle);
		}

		static bool isAssetLoaded(AssetHandle handle)
		{
			return Project::getActive()->getAssetManager()->isAssetLoaded(handle);
		}
	};

}