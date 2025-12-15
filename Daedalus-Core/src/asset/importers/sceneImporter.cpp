#include "ddpch.h"
#include "sceneImporter.h"

#include "../../project/project.h"
#include "../../scene/sceneSerializer.h"

namespace daedalus {

	Shr_ptr<scene::Scene> SceneImporter::importScene(AssetHandle handle, const AssetMetadata& metadata)
	{
		return loadScene(Project::getActiveAssetDirectory() / metadata.filepath);
	}

	Shr_ptr<scene::Scene> SceneImporter::loadScene(const std::filesystem::path& path)
	{
		auto newScene = create_shr_ptr<scene::Scene>();

		scene::SceneSerializer serializer(newScene);
		if (serializer.deserialize(path))
			return newScene;

		DD_CORE_LOG_ERROR("Failed to load scene: {}", path);
		return nullptr;
	}

	void SceneImporter::saveScene(Shr_ptr<scene::Scene> scene, const std::filesystem::path& path)
	{
		scene::SceneSerializer serializer(scene);
		serializer.serialize(path);

		auto relPath = std::filesystem::relative(path, Project::getActiveAssetDirectory());
		if (relPath.empty())
			relPath = path;

		AssetHandle handle = Project::getActive()->getEditorAssetManager()->getHandleWithFilepath(relPath);
		if (handle)
			Project::getActive()->getEditorAssetManager()->reimportAsset(handle);
		else
			Project::getActive()->getEditorAssetManager()->importAsset(relPath);
	}

}