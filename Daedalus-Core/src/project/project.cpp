#include "ddpch.h"
#include "project.h"

#include "projectSerializer.h"

namespace daedalus {

	Shr_ptr<Project> Project::newProject()
	{
		s_activeProject = create_shr_ptr<Project>();
		return s_activeProject;
	}

	Shr_ptr<Project> Project::Load(const std::filesystem::path& path)
	{
		Shr_ptr<Project> project = create_shr_ptr<Project>();

		ProjectSerializer serializer(project);
		if (serializer.deserialize(path))
		{
			project->m_projectDirectory = path.parent_path();
			s_activeProject = project;
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