#pragma once

#include "../../scene/scene.h"
#include "../assetMetadata.h"

namespace daedalus {

	class SceneImporter
	{
	public:
		static IntrusivePtr<scene::Scene> importScene(AssetHandle handle, const AssetMetadata& metadata);

		static IntrusivePtr<scene::Scene> loadScene(const std::filesystem::path& path);

		static void saveScene(IntrusivePtr<scene::Scene> scene, const std::filesystem::path& path);
	};

}