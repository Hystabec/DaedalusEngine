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
		static IntrusivePtr<T> getAsset(AssetHandle handle)
		{
			IntrusivePtr<Asset> asset = Project::getActive()->getAssetManager()->getAsset(handle);
			return static_pointer_cast<T>(asset);
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