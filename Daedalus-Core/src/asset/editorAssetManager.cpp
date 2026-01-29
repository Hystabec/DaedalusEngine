#include "ddpch.h"
#include "editorAssetManager.h"

#include "importers/assetImporter.h"
#include "../project/project.h"
#include "../utils/fileUtils.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace daedalus {

	static std::map<std::filesystem::path, AssetType> s_assetExtensionMap = {
		{ ".ddscene", AssetType::Scene },
		{ ".png", AssetType::Texture2D }, { ".jpg", AssetType::Texture2D }, { ".jpeg", AssetType::Texture2D }
	};

	static AssetType get_asset_type_from_extension(const std::filesystem::path& extension)
	{
		std::filesystem::path lowerExtension = utils::FileSystem::fileExtensionToLower(extension);
		if (s_assetExtensionMap.contains(lowerExtension))
			return s_assetExtensionMap[lowerExtension];

		DD_CORE_LOG_WARN("could not find asset type for extension {}", extension);
		return AssetType::None;
	}

	IntrusivePtr<Asset> daedalus::EditorAssetManager::getAsset(AssetHandle handle)
	{
		if (!isAssetHandleValid(handle))
			return nullptr;

		IntrusivePtr<Asset> asset;
		if (isAssetLoaded(handle))
			asset = m_loadedAssets.at(handle);
		else
		{
			const AssetMetadata& metadata = getMetadata(handle);
			asset = AssetImporter::importAsset(handle, metadata);
			if (!asset)
				DD_CORE_LOG_ERROR("EditorAssetManager::getAsset - Import failed");
			else
				m_loadedAssets[handle] = asset;
		}

		return asset;
	}

	AssetType EditorAssetManager::getAssetType(AssetHandle handle) const
	{
		if (!isAssetHandleValid(handle))
		{
			DD_CORE_LOG_ERROR("EditorAssetManager::getAssetType - Invalid asset handle {}", handle);
			return AssetType::None;
		}

		return m_assetRegistry.at(handle).type;
	}

	bool daedalus::EditorAssetManager::isAssetHandleValid(AssetHandle handle) const
	{
		return handle != 0 && m_assetRegistry.contains(handle);
	}

	bool daedalus::EditorAssetManager::isAssetLoaded(AssetHandle handle) const
	{
		return m_loadedAssets.contains(handle);
	}

	AssetHandle EditorAssetManager::importAsset(const std::filesystem::path& filepath, bool overridePrevious)
	{
		// TO DO: Multi-thread this to speed up the importing of assets - openGL makes this 
		// currently a little complicated as openGL commands need to be run by the main thread

		// check that the asset doesnt already exist in the registy
		AssetHandle foundHandle = getHandleWithFilepath(filepath);
		AssetHandle handle; // generate a new handle

		if(foundHandle) {
			if (!overridePrevious)
				return foundHandle;
			else
				handle = foundHandle;
		}

		AssetMetadata metadata;
		metadata.filepath = filepath;
		metadata.type = get_asset_type_from_extension(filepath.extension());
		DD_CORE_ASSERT(metadata.type != AssetType::None);
		IntrusivePtr<Asset> asset = AssetImporter::importAsset(handle, metadata);
		if (asset)
		{
			asset->handle = handle;
			m_loadedAssets[handle] = asset;
			m_assetRegistry[handle] = metadata;
			serializeAssetRegistry();
			return handle;
		}
		return AssetHandle(0);
	}

	bool EditorAssetManager::reimportAsset(AssetHandle handle)
	{
		if (handle == 0)
			return false;

		const AssetMetadata& metadata = getMetadata(handle);
		IntrusivePtr<Asset> asset = AssetImporter::importAsset(handle, metadata);
		if (asset)
		{
			m_loadedAssets[handle] = asset;
			return true;
		}

		return false;
	}

	const AssetMetadata& daedalus::EditorAssetManager::getMetadata(AssetHandle handle) const
	{
		static AssetMetadata s_nullMetadata;
		auto it = m_assetRegistry.find(handle);
		if (it == m_assetRegistry.end())
			return s_nullMetadata;

		return it->second;
	}

	const std::filesystem::path& EditorAssetManager::getFilepath(AssetHandle handle) const
	{
		const AssetMetadata& metadata = getMetadata(handle);
		return metadata.filepath;
	}

	void EditorAssetManager::serializeAssetRegistry()
	{
		// NOTE : consider adding a file version like scene files to 
		// make backwards compatibility easier

		auto path = Project::getActive()->getConfig().assetRegistryPath;

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetRegistry" << YAML::Value;

		out << YAML::BeginSeq;
		for (const auto& [handle, metadata] : m_assetRegistry)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Handle" << YAML::Value << handle;
			out << YAML::Key << "Filepath" << YAML::Value << metadata.filepath.generic_string();
			out << YAML::Key << "Type" << YAML::Value << asset_type_to_string(metadata.type);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

	bool EditorAssetManager::deserializeAssetRegistry()
	{
		auto path = Project::getActiveProjectDirectory() / Project::getActive()->getConfig().assetRegistryPath;

		if (!std::filesystem::exists(path))
		{
			DD_CORE_LOG_WARN("Asset registry file not found at {}", path);
			return false;
		}

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch(YAML::Exception e)
		{
			DD_CORE_LOG_ERROR("Failed to load project asset registry ({}): {}", path, e.what());
			return false;
		}

		auto rootNode = data["AssetRegistry"];
		if (!rootNode)
			return false;

		for (const auto& node : rootNode)
		{
			AssetHandle handle = node["Handle"].as<uint64_t>();
			auto& metadata = m_assetRegistry[handle];
			metadata.filepath = node["Filepath"].as<std::string>();
			metadata.type = asset_type_from_string(node["Type"].as<std::string>());
		}

		return true;
	}

	AssetHandle EditorAssetManager::getHandleWithFilepath(const std::filesystem::path& filepath)
	{
		for (auto& [handle, metadata] : m_assetRegistry)
		{
			if (filepath == metadata.filepath)
				return handle;
		}

		return AssetHandle(0);
	}

}