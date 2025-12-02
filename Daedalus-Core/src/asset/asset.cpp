#include "ddpch.h"
#include "asset.h"

namespace daedalus {

	std::string_view asset_type_to_string(AssetType type)
	{
		switch (type)
		{
		case daedalus::AssetType::None:
			return "AssetType::None";
		case daedalus::AssetType::Scene:
			return "AssetType::Scene";
		case daedalus::AssetType::Texture2D:
			return "AssetType::Texture2D";
		}

		return "AssetType::<Invalid>";
	}

	AssetType asset_type_from_string(std::string_view assetType)
	{
		if (assetType == "AssetType::None")
			return AssetType::None;
		if (assetType == "AssetType::Scene")
			return AssetType::Scene;
		if (assetType == "AssetType::Texture2D")
			return AssetType::Texture2D;

		return AssetType::None;
	}

}