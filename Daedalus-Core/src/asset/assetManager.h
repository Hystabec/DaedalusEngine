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

		// If the asset type is different to the castType an invalid asset will be returned
		template<typename T>
		static IntrusivePtr<T> getAsset(AssetHandle handle, AssetType castType = AssetType::None)
		{
			IntrusivePtr<Asset> asset = Project::getActive()->getAssetManager()->getAsset(handle);

			if(castType != AssetType::None && asset->getType() != castType)
				return nullptr;

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