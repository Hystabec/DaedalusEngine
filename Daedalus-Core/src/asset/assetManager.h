#pragma once

#include "assetManagerBase.h"
#include "../project/project.h"

namespace daedalus {

	class AssetManager
	{
	public:
		template<typename T>
		static Shr_ptr<T> getAsset(AssetHandle handle)
		{
			Shr_ptr<Asset> asset = Project::getActive()->getAssetManager()->getAsset(handle);
			return std::static_pointer_cast<T>(asset);
		}
	};

}