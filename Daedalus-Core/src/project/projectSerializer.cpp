#include "ddpch.h"
#include "projectSerializer.h"
#include "../utils/engineYAMLSerializers.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace daedalus {

	ProjectSerializer::ProjectSerializer(IntrusivePtr<Project> project)
		: m_project(project)
	{
	}

	bool ProjectSerializer::serialize(const std::filesystem::path& path)
	{
		const auto& config = m_project->getConfig();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "FormatVersion" << YAML::Value << m_currentFileFormatVersion;
		out << YAML::Key << "Project" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << config.name;

		out << YAML::Key << "AssetDirectory" << YAML::Value << config.assetDirectory.generic_string();
		out << YAML::Key << "AssetRegistryPath" << YAML::Value << config.assetRegistryPath.generic_string();
		out << YAML::Key << "ScriptModuleBin" << YAML::Value << config.scriptModuleBin.generic_string();
		out << YAML::Key << "StartScene" << YAML::Value << config.startScene;

		out << YAML::EndMap;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();

		return true;
	}

	bool ProjectSerializer::deserialize(const std::filesystem::path& path)
	{
		auto& config = m_project->getConfig();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (YAML::Exception e)
		{
			DD_CORE_LOG_ERROR("Failed to load project file '{}'\n{}", path, e.what());
			return false;
		}

		YAML::Node projectNode;
		try
		{
			if (!data["Project"])
			{
				DD_CORE_LOG_ERROR("Deserialization Error: file contains no project data '{}'", path);
				return false;
			}
			else
				projectNode = data["Project"];
		}
		catch (YAML::Exception e)
		{
			DD_CORE_LOG_ERROR("Failed to load project file '{}'\n{}", path, e.what());
			return false;
		}

		uint32_t fileVersion = data["FormatVersion"].as<uint32_t>();

		config.name = projectNode["Name"].as<std::string>();
		config.assetDirectory = projectNode["AssetDirectory"].as<std::string>();
		if (projectNode["AssetRegistryPath"])
			config.assetRegistryPath = projectNode["AssetRegistryPath"].as<std::string>();
		else
			config.assetRegistryPath = "assetsRegistry.ddreg"; // Default reg pat/name
		config.scriptModuleBin = projectNode["ScriptModuleBin"].as<std::string>();
		config.startScene = projectNode["StartScene"].as<AssetHandle>();

		return true;
	}

}