#include "ddpch.h"
#include "project.h"

#include "projectSerializer.h"

namespace daedalus {

	IntrusivePtr<Project> Project::newProject()
	{
		s_activeProject = make_intrusive_ptr<Project>();
		return s_activeProject;
	}

	IntrusivePtr<Project> Project::Load(const std::filesystem::path& path)
	{
		IntrusivePtr<Project> project = make_intrusive_ptr<Project>();

		ProjectSerializer serializer(project);
		if (serializer.deserialize(path))
		{
			project->m_projectDirectory = path.parent_path();
			s_activeProject = project;

			// check that the config paths are valid (not empty)
			// TO DO: Reconsider doing this as it does mean that dirs / files have to be (a / in a)
			// sub folder and can never be placed in the root project dir
			ProjectConfig& config = s_activeProject->getConfig();
			if (config.assetDirectory.empty())
				config.assetDirectory = "assets";
			if (config.assetRegistryPath.empty())
				config.assetRegistryPath = "engine\\assetsRegistry.ddreg";
			if (config.scriptModuleBin.empty())
				config.scriptModuleBin = "engine\\userScripting\\script-bin\\UserScripting.dll"; // NOTE: this makes the default file name userScripting.dll
			if (config.logDirectory.empty())
				config.logDirectory = "logs";

			// check that the specified project config directories exist, if not create them
			// TO DO: check if I will also need to create the parent dirs for bin/reg fils
			if (!std::filesystem::exists(s_activeProject->relativePath(config.assetDirectory)))
				std::filesystem::create_directories(s_activeProject->relativePath(config.assetDirectory));
			if (!std::filesystem::exists(s_activeProject->relativePath(config.logDirectory)))
				std::filesystem::create_directories(s_activeProject->relativePath(config.logDirectory));

			std::shared_ptr<EditorAssetManager> editorAssetManager = std::make_shared<EditorAssetManager>();
			s_activeProject->m_assetManager = editorAssetManager;
			editorAssetManager->deserializeAssetRegistry();
			return s_activeProject;
		}

		return nullptr;
	}

	bool Project::saveActive(const std::filesystem::path& path)
	{
		ProjectSerializer serializer(s_activeProject);
		if (serializer.serialize(path))
		{
			s_activeProject->m_projectDirectory = path.parent_path();
			return true;
		}

		return false;
	}

}