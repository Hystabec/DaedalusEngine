#pragma once

#include "assetManagerBase.h"
#include "assetMetadata.h"

#include <map>

namespace daedalus {

	// NOTE: This could potentialy be made into an unordered_map (and probably should)
	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;

	class EditorAssetManager : public AssetManagerBase
	{
	public:
		Shr_ptr<Asset> getAsset(AssetHandle handle) const override;

		bool isAssetHandleValid(AssetHandle handle) const override;
		bool isAssetLoaded(AssetHandle handle) const override;
		 
		AssetHandle importAsset(const std::filesystem::path& filepath);

		const AssetMetadata& getMetadata(AssetHandle handle) const;

		const AssetRegistry& getAssetRegistry() const { return m_assetRegistry; }

		void serializeAssetRegistry();
		bool deserializeAssetRegistry();
	private:
		/// @brief returns an invalid handle if one was not found
		AssetHandle getHandleWithFilepath(const std::filesystem::path& filepath);
	private:
		AssetRegistry m_assetRegistry;
		AssetMap m_loadedAssets;

		// TO DO: memory-only assets
	};

}