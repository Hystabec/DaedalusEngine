#pragma once

#include <string>
#include <filesystem>

namespace daedalus {

	struct ProjectConfig
	{
		std::string name = "Untitled";

		std::filesystem::path assetDirectory;
		std::filesystem::path scriptModuleBin;

		std::filesystem::path startScene;
	};

	class Project
	{
	public:
		Project() {};

		ProjectConfig& getConfig() { return m_config; }
	
		static Shr_ptr<Project> getActive() { return s_activeProject; }

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

		// TO DO: move to asset manager
		static std::filesystem::path getAssetFileSystemPath(const std::filesystem::path& path)
		{
			DD_CORE_ASSERT(s_activeProject);
			return getActiveAssetDirectory() / path;
		}

		static Shr_ptr<Project> newProject();
		static Shr_ptr<Project> Load(const std::filesystem::path& path);
		static bool saveActive(const std::filesystem::path& path);

	private:
		ProjectConfig m_config;
		std::filesystem::path m_projectDirectory;

		inline static Shr_ptr<Project> s_activeProject;
	};

}