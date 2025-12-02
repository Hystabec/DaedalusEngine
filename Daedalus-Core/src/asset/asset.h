#pragma once

#include "../application/uuid.h"

namespace daedalus {

	using AssetHandle = UUID;

	enum class AssetType : uint16_t
	{
		None = 0,
		Scene,
		Texture2D
	};

	std::string_view asset_type_to_string(AssetType type);
	AssetType asset_type_from_string(std::string_view assetType);

	class Asset
	{
	public:
		AssetHandle handle; // generate handle

		virtual AssetType getType() const = 0;
	};

}