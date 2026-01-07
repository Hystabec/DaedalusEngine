#pragma once

#include "assetManagerBase.h"
#include "assetMetadata.h"

#include <map>

namespace daedalus {

	// NOTE: This could potentialy be made into an unordered_map
	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;

	class EditorAssetManager : public AssetManagerBase
	{
	public:
		bool isAssetHandleValid(AssetHandle handle) const override;

		IntrusivePtr<Asset> getAsset(AssetHandle handle) override;
		AssetType getAssetType(AssetHandle handle) const override;
		bool isAssetLoaded(AssetHandle handle) const override;
		 
		AssetHandle importAsset(const std::filesystem::path& filepath, bool overridePrevious = false);
		bool reimportAsset(AssetHandle handle);

		/// @brief returns an invalid handle if one was not found
		AssetHandle getHandleWithFilepath(const std::filesystem::path& filepath);

		const AssetMetadata& getMetadata(AssetHandle handle) const;
		const std::filesystem::path& getFilepath(AssetHandle handle) const;

		const AssetRegistry& getAssetRegistry() const { return m_assetRegistry; }

		void serializeAssetRegistry();
		bool deserializeAssetRegistry();
	private:
		AssetRegistry m_assetRegistry;
		AssetMap m_loadedAssets;

		// TO DO: memory-only assets
	};

}