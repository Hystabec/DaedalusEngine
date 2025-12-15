#pragma once

#include "../../scene/scene.h"
#include "../assetMetadata.h"

namespace daedalus {

	class SceneImporter
	{
	public:
		static Shr_ptr<scene::Scene> importScene(AssetHandle handle, const AssetMetadata& metadata);

		static Shr_ptr<scene::Scene> loadScene(const std::filesystem::path& path);

		static void saveScene(Shr_ptr<scene::Scene> scene, const std::filesystem::path& path);
	};

}