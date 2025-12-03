#include "ddpch.h"
#include "editorAssetManager.h"

#include "assetImporter.h"
#include "../project/project.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace daedalus {

	Shr_ptr<Asset> daedalus::EditorAssetManager::getAsset(AssetHandle handle) const
	{
		if (!isAssetHandleValid(handle))
			return nullptr;

		Shr_ptr<Asset> asset;
		if (isAssetLoaded(handle))
			asset = m_loadedAssets.at(handle);
		else
		{
			const AssetMetadata& metadata = getMetadata(handle);
			asset = AssetImporter::importAsset(handle, metadata);
			if (!asset)
				DD_CORE_LOG_ERROR("EditorAssetManager::getAsset - Import failed");

		}

		return asset;
	}

	bool daedalus::EditorAssetManager::isAssetHandleValid(AssetHandle handle) const
	{
		return handle != 0 && m_assetRegistry.contains(handle);
	}

	bool daedalus::EditorAssetManager::isAssetLoaded(AssetHandle handle) const
	{
		return m_loadedAssets.contains(handle);
	}

	AssetHandle EditorAssetManager::importAsset(const std::filesystem::path& filepath)
	{
		// TO DO: Multi-thread this to speed up the importing of assets - openGL makes this 
		// currently a little complicated as openGL commands need to be run by the main thread

		// check that the asset doesnt already exist in the registy
		{
			AssetHandle foundHandle = getHandleWithFilepath(filepath);
			if (foundHandle)
				return foundHandle;
		}

		AssetHandle handle; // generate a new handle
		AssetMetadata metadata;
		metadata.filepath = filepath;
		metadata.type = AssetType::Texture2D; // TO DO: get from file extension
		Shr_ptr<Asset> asset = AssetImporter::importAsset(handle, metadata);
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

	const AssetMetadata& daedalus::EditorAssetManager::getMetadata(AssetHandle handle) const
	{
		static AssetMetadata s_nullMetadata;
		auto it = m_assetRegistry.find(handle);
		if (it == m_assetRegistry.end())
			return s_nullMetadata;

		return it->second;
	}

	void EditorAssetManager::serializeAssetRegistry()
	{
		// NOTE : consider adding a file version like scene files to 
		// make backwards compatibility easier

		auto path = Project::getActiveAssetRegistryPath();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetRegistry" << YAML::Value;

		out << YAML::BeginSeq;
		for (const auto& [handle, metadata] : m_assetRegistry)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Handle" << YAML::Value << handle;
			std::string filePathStr = metadata.filepath.generic_string();
			out << YAML::Key << "Filepath" << YAML::Value << filePathStr;
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
		auto path = Project::getActiveAssetRegistryPath();
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch(YAML::ParserException e)
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