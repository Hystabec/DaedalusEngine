#pragma once

#include "asset.h"

#include <filesystem>

namespace daedalus
{

	struct AssetMetadata
	{
		AssetType type = AssetType::None;
		std::filesystem::path filepath;

		operator bool() const { return type != AssetType::None; }
	};

}