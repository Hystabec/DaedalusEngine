#pragma once

#include "project.h"

namespace daedalus {

	class ProjectSerializer
	{
	public:
		ProjectSerializer(IntrusivePtr<Project> project);

		bool serialize(const std::filesystem::path& path);
		bool deserialize(const std::filesystem::path& path);

	private:
		IntrusivePtr<Project> m_project;
		static constexpr uint32_t m_currentFileFormatVersion = 1;
	};

}