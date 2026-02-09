#pragma once

//#include "../asset/runtimeAssetManager.h"
#include "../asset/editorAssetManager.h"

#include <string>
#include <filesystem>

namespace daedalus {

	struct ProjectConfig
	{
		std::string name = "Untitled";

		std::filesystem::path assetDirectory;
		std::filesystem::path assetRegistryPath;
		std::filesystem::path scriptModuleBin;
		std::filesystem::path logDirectory;

		AssetHandle startScene;
	};

	class Project : public IntrusiveCounter
	{
	public:
		Project() {};

		ProjectConfig& getConfig() { return m_config; }
	
		static IntrusivePtr<Project> getActive() { return s_activeProject; }

		std::shared_ptr<AssetManagerBase> getAssetManager() { return m_assetManager; }
		//std::shared_ptr<RuntimeAssetManager> getRuntimeAssetManager() { return std::static_pointer_cast<RuntimeAssetManager>(m_assetManager); }
		std::shared_ptr<EditorAssetManager> getEditorAssetManager() { return std::static_pointer_cast<EditorAssetManager>(m_assetManager); }

		static const std::filesystem::path& getActiveProjectDirectory()
		{
			DD_CORE_ASSERT(s_activeProject);
			return s_activeProject->m_projectDirectory;
		}

		static std::filesystem::path getActiveAssetDirectory() 
		{ 
			DD_CORE_ASSERT(s_activeProject);
			return getActiveProjectDirectory() / s_activeProject->m_config.assetDirectory;
		}

		/*static std::filesystem::path getActiveAssetRegistryPath()
		{
			DD_CORE_ASSERT(s_activeProject);
			return getActiveAssetDirectory() / s_activeProject->m_config.assetRegistryPath;
		}*/

		// TO DO: move to asset manager
		static std::filesystem::path getAssetFileSystemPath(const std::filesystem::path& path)
		{
			DD_CORE_ASSERT(s_activeProject);
			return getActiveAssetDirectory() / path;
		}

		std::filesystem::path relativePath(const std::filesystem::path& path) const
		{
			return m_projectDirectory / path;
		}

		/// @param directoryPath - The path to the directory where the new project will be created
		static IntrusivePtr<Project> newProject(const std::filesystem::path& directoryPath, const std::string& projectName);
		static IntrusivePtr<Project> Load(const std::filesystem::path& path);
		static bool saveActive(const std::filesystem::path& path);

	private:
		ProjectConfig m_config;
		std::filesystem::path m_projectDirectory;
		std::shared_ptr<AssetManagerBase> m_assetManager;

		inline static IntrusivePtr<Project> s_activeProject;
	};

}