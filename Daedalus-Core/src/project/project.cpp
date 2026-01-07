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